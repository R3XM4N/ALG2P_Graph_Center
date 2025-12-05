#include "../include/graph.hpp"

void Graph::AddNode(uint64 node_indentifier){
    if (this->node_to_index.find(node_indentifier) == this->node_to_index.end()){
        this->node_ptr_mapping.push_back(std::make_shared<Node>());
        this->node_ptr_mapping.back()->eccentricity = 0;
        this->node_ptr_mapping.back()->id = node_indentifier;
        this->node_to_index[node_indentifier] = this->node_ptr_mapping.size() - 1;
        this->neighbors.push_back(vector<uint32>());
    }
}

void Graph::AddEdge(uint64 node_a_indentifier, uint64 node_b_indentifier){
    if (this->node_to_index.find(node_a_indentifier) == this->node_to_index.end()){
        AddNode(node_a_indentifier);
    }
    if (this->node_to_index.find(node_b_indentifier) == this->node_to_index.end()){
        AddNode(node_b_indentifier);
    }
    this->neighbors[node_to_index[node_a_indentifier]].push_back(node_to_index[node_b_indentifier]);
    this->neighbors[node_to_index[node_b_indentifier]].push_back(node_to_index[node_a_indentifier]);
}

uint64 Graph::GetMinDistance(shared_ptr<Node> source_node, shared_ptr<Node> destination_node){
    return GetMinDistance(this->node_to_index[source_node->id], this->node_to_index[destination_node->id]);
}

uint64 Graph::GetMinDistance(uint32 dense_source_node_id, uint32 dense_destination_node_id){
    if (dense_source_node_id == dense_destination_node_id) return 0;
    vector<bool> visited;
    for (size_t i = 0; i < this->neighbors.size(); i++){
        if (dense_source_node_id == i){
            visited.push_back(1);
            continue;
        }
        visited.push_back(0);
    }
    
    queue<uint32> node_q;
    uint32 c_steps = 0;
    uint32 active_size = 1;
    node_q.push(dense_source_node_id);

    while (active_size = node_q.size(), active_size > 0){
        for (uint32 i = 0; i < active_size; i++){
            if (node_q.front() == dense_destination_node_id){
                return c_steps;
            }
            for (const uint32 &n_id : this->neighbors[node_q.front()]){
                if (visited[n_id]){
                    continue;
                }
                visited[n_id] = 1;
                node_q.push(n_id);
            }
            node_q.pop();
        }
        c_steps++;
    }
    return 0 - 1;
}

/// @brief Hides threading
namespace Detail{
    void SubEdge(const vector<vector<uint32>> &neighbors, const shared_ptr<Node> &node, const uint32 &src_id){
        const uint32 nodes_count = neighbors.size();
        std::vector<int32_t> distance(nodes_count, -1);
        std::fill(distance.begin(), distance.end(), -1);
        std::queue<uint32> bfs;

        bfs.push(src_id);
        distance[src_id] = 0;
        uint64 max_dist = 0;

        while (!bfs.empty()) {
            const uint32 cur_node = bfs.front();
            bfs.pop();

            const int32_t cur_distance = distance[cur_node];
            if (static_cast<uint64>(cur_distance) > max_dist)
                max_dist = static_cast<uint64>(cur_distance);

            for (const auto neighbor : neighbors[cur_node]) {
                if (distance[neighbor] == -1) {
                    distance[neighbor] = cur_distance + 1;
                    bfs.push(neighbor);
                }
            }
        }
        node->eccentricity = max_dist;
    }
}

void Graph::Edging()
{
    if (this->max_threads > 2){
        vector<std::thread> threads;
        const uint32 nodes_count = this->neighbors.size();
    
        for (uint32 src_id = 0; src_id < nodes_count; ++src_id){
            threads.emplace_back(Detail::SubEdge, std::cref(this->neighbors), this->node_ptr_mapping[src_id], src_id); //HOLY CONST REF MAKES IT SO MUCH FASTER
            if (threads.size() == this->max_threads){
                for (unsigned short thread = 0; thread < this->max_threads; thread++){
                    threads[thread].join();
                }
                threads.clear();
            }
        }
        //uneven
        for (auto &thread : threads){
            if (thread.joinable())
                thread.join();
        }
        return;    
    }

    const uint32 nodes_count = this->neighbors.size();
    std::vector<int32_t> distance(nodes_count, -1);

    for (uint32 src_id = 0; src_id < nodes_count; ++src_id) {
        std::fill(distance.begin(), distance.end(), -1);
        std::queue<uint32> bfs;

        bfs.push(src_id);
        distance[src_id] = 0;
        uint64 max_dist = 0;

        while (!bfs.empty()) {
            const uint32 cur_node = bfs.front();
            bfs.pop();

            const int32_t cur_distance = distance[cur_node];
            if (static_cast<uint64>(cur_distance) > max_dist)
                max_dist = static_cast<uint64>(cur_distance);

            for (const auto neighbor : this->neighbors[cur_node]) {
                if (distance[neighbor] == -1) {
                    distance[neighbor] = cur_distance + 1;
                    bfs.push(neighbor);
                }
            }
        }
        node_ptr_mapping[src_id]->eccentricity = max_dist;
    }
}

vector<shared_ptr<Node>> Graph::GetCenter(){
    Edging();
    vector<shared_ptr<Node>> potentials;
    uint64 best = 0 - 1;
    // for (size_t i = 0; i < this->node_ptr_mapping.size(); i++)
    // {
    //     std::cout << this->node_ptr_mapping[i]->id <<" " << this->node_ptr_mapping[i]->eccentricity << "\n";
    // }
    
    for (const shared_ptr<Node> &node : this->node_ptr_mapping){
        if (node->eccentricity < best){
            potentials.clear();
            best = node->eccentricity;
            potentials.push_back(node);
            continue;
        }
        if (node->eccentricity == best){
            potentials.push_back(node);
        }
    }
    return potentials;
}

void Graph::LoadFromFFileTXT(const std::string &file_location){
    std::ifstream stream(file_location);
    std::string buffer;
    uint64 active_neighbor_id, active_node_id;
    while (getline (stream, buffer)){
        std::stringstream text(buffer);
        text >> active_node_id >> active_neighbor_id;
        AddEdge(active_node_id, active_neighbor_id);
    }
    stream.close();
}

void Graph::AvaibleThreads(unsigned short int new_thread_max){
    this->max_threads = new_thread_max;
}

Graph::Graph(){

}
Graph::~Graph(){

}
