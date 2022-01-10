//
// Created by Roy Levi on 05/08/2020.
//

#include "graph.h"
#include <iostream>

#define UNDEFINED_GRAPH "Error: given graph does not exist"

// FOR PYTHON USAGE

DirectedGraph* create() {
    auto *graph = new DirectedGraph();
    return graph;
}

void destroy(DirectedGraph* graph) {
    if (graph == nullptr) {
        std::cout << UNDEFINED_GRAPH << std::endl;
        return;
    }
    delete graph;
}

DirectedGraph* addVertex(DirectedGraph *graph, const char* v) {
    if (graph == nullptr) {
        std::cout << UNDEFINED_GRAPH << std::endl;
        return nullptr;
    }

    Vertex vertex(v);

    if (graph->containsVertex(vertex)) {
        std::cout << DirectedGraph::IllegalVertex(vertex, "Vertex name must be unique").what() << std::endl;
        return nullptr;
    }

    std::set<Vertex> vertices = {vertex};
    *graph = *graph+DirectedGraph(vertices);
    return graph;
}

DirectedGraph* addEdge(DirectedGraph *graph, const char* v1, const char* v2) {
    if (graph == nullptr) {
        std::cout << UNDEFINED_GRAPH << std::endl;
        return nullptr;
    }

    Vertex vertex1(v1), vertex2(v2);
    DirectedEdge edge(vertex1, vertex2);

    if (!graph->containsVertex(vertex1)) {
        std::cout << DirectedGraph::IllegalEdge(edge, "Expecting source vertex to exist").what() << std::endl;
        return nullptr;
    }
    if (!graph->containsVertex(vertex2)) {
        std::cout << DirectedGraph::IllegalEdge(edge, "Expecting destination vertex to exist").what() << std::endl;
        return nullptr;
    }
    if (vertex1 == vertex2) {
        std::cout << DirectedGraph::IllegalEdge(edge, "Self loop edges are not allowed").what() << std::endl;
        return nullptr;
    }
    if (graph->containsEdge(edge)) {
        std::cout << DirectedGraph::IllegalEdge(edge, "Parallel edges are not allowed").what() << std::endl;
        return nullptr;
    }

    std::set<Vertex> vertices = {vertex1, vertex2};
    std::set<DirectedEdge> edges = {edge};
    *graph = *graph+DirectedGraph(vertices, edges);
    return graph;
}

void disp(DirectedGraph *graph) {
    if (graph == nullptr) {
        std::cout << UNDEFINED_GRAPH << std::endl;
        return;
    }
    graph->print(std::cout);
}

DirectedGraph* graphUnion(DirectedGraph *graph_in1, DirectedGraph *graph_in2, DirectedGraph *graph_out) {
    if (graph_in1 == nullptr || graph_in2 == nullptr || graph_out == nullptr) {
        std::cout << UNDEFINED_GRAPH << std::endl;
        return nullptr;
    }
    *graph_out = *graph_in1 + *graph_in2;
    return graph_out;
}

DirectedGraph *graphIntersection(DirectedGraph *graph_in1, DirectedGraph *graph_in2, DirectedGraph *graph_out) {
    if (graph_in1 == nullptr || graph_in2 == nullptr || graph_out == nullptr) {
        std::cout << UNDEFINED_GRAPH << std::endl;
        return nullptr;
    }
    *graph_out = *graph_in1 ^ *graph_in2;
    return graph_out;
}

DirectedGraph *graphDifference(DirectedGraph *graph_in1, DirectedGraph *graph_in2, DirectedGraph *graph_out) {
    if (graph_in1 == nullptr || graph_in2 == nullptr || graph_out == nullptr) {
        std::cout << UNDEFINED_GRAPH << std::endl;
        return nullptr;
    }
    *graph_out = *graph_in1 - *graph_in2;
    return graph_out;
}

DirectedGraph *graphProduct(DirectedGraph *graph_in1, DirectedGraph *graph_in2, DirectedGraph *graph_out) {
    if (graph_in1 == nullptr || graph_in2 == nullptr || graph_out == nullptr) {
        std::cout << UNDEFINED_GRAPH << std::endl;
        return nullptr;
    }
    *graph_out = *graph_in1 * *graph_in2;
    return graph_out;
}

DirectedGraph *graphComplement(DirectedGraph *graph_in, DirectedGraph *graph_out) {
    if (graph_in == nullptr || graph_out == nullptr) {
        std::cout << UNDEFINED_GRAPH << std::endl;
        return nullptr;
    }
    *graph_out = !(*graph_in);
    return graph_out;
}
