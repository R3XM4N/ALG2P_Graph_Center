#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <algorithm>

using uint64 = unsigned long long int;
using uint32 = unsigned long int;


/// @brief Represents a single node/edge of a graph
struct Node {
    /// @brief id/name of the node
    uint64 id = 0;
    /// @brief eccentricity of the node
    uint64 eccentricity = 0;
};

/// @brief Class representing a graph
class Graph
{
private:
    /// @brief node to it's dense indec
    std::unordered_map<uint64, uint32> node_to_index;
    /// @brief adjcated list for neighbors uses dense indeces
    std::vector<std::vector<uint32>> neighbors;
    /// @brief vector uses dense indeces to match to nodes
    std::vector<std::shared_ptr<Node>> node_ptr_mapping;
    /// @brief Adds the node into the system if it doesn't exist
    /// @param node_indentifier The identifier of the node that should be added
    void AddNode(uint64 node_indentifier);
    /// @brief Adds the edge between nodes
    /// @param node_a_indentifier Identifier of node a of the ones that should the edge exist between
    /// @param node_b_indentifier Identifier of node b of the ones that should the edge exist between
    void AddEdge(uint64 node_a_indentifier, uint64 node_b_indentifier);
    /// @brief Recalculates all nodes eccentricity
    void Edging();
    /// @brief Maximum ammount of threads to run on
    unsigned short max_threads = 1;
public:
    /// @brief Alternative call of GetMinDistance() with shared_ptrs
    /// @param source_node Source node
    /// @param destination_node Destination node
    /// @return Returns the minimum Distance betweeen these nodes
    uint64 GetMinDistance(std::shared_ptr<Node> source_node, std::shared_ptr<Node> destination_node);
    /// @brief Gets minimum distance between two nodes using bfs
    /// @param source_node_id source node id
    /// @param destination_node_id destination node id
    /// @return Returns the minimum Distance betweeen these nodes
    uint64 GetMinDistance(uint32 dense_source_node_id, uint32 dense_destination_node_id);
    /// @brief Returns the center node(s) of the graph
    /// @return Returns either singular list of the cented node or a multiple member list of all nodes qualifying as center
    std::vector<std::shared_ptr<Node>> GetCenter();
    /// @brief constructor
    Graph();
    /// @brief destructor
    ~Graph();
    /// @brief Loads data from the "formated" text file into the graph 
    /// @param file_location Location of the file in the file system
    void LoadFromFFileTXT(const std::string &file_location);
    /// @brief Sets the ammount of avaible threads for the program
    /// @param new_thread_max ammount of maximum avaible work threads 
    void AvaibleThreads(unsigned short int new_thread_max);
};


#endif