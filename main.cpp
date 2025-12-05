#include <iostream>
#include <string>

#include "include/graph.hpp"

int main(int argc, char** argv){
    if (argc < 2 || argc > 3){
        std::cerr << "\e[0;31mINVALID ARGUMENT COUNT.\e[0m\n";
    }
            
    Graph graph;
    graph.LoadFromFFileTXT(argv[1]);
    // std::cout << "Graph loaded\n";
    if (argc > 2)
        graph.AvaibleThreads(static_cast<unsigned short>(std::stoi(argv[2])));

    vector<shared_ptr<Node>> centered = graph.GetCenter();
    std::cout << "Center nodes:\n";
    for (auto &i : centered){
        std::cout << "Node: " << i.get()->id 
            << " Eccentricity: " << i.get()->eccentricity << "\n";
    }
    return EXIT_SUCCESS;
}