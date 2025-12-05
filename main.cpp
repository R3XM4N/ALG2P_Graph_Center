#include <iostream>
#include <memory>

#include "include/graph.hpp"

int main(int argc, char** argv){
    if (argc < 2 || argc > 2){
        std::cerr << "\e[0;31mINVALID ARGUMENT COUNT.\e[0m\n";
    }
    Graph graph;
    graph.LoadFromFFileTXT(argv[1]);
    std::cout << "loaded\n";
    vector<std::shared_ptr<Node>> centered = graph.GetCenter();
    for (auto &i : centered){
        std::cout << i.get()->id << " " << i.get()->eccentricity << "\n";
    }
    return EXIT_SUCCESS;
}