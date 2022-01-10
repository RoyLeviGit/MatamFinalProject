//
// Created by Roy Levi on 03/08/2020.
//

#include "DirectedGraph.h"

#include <utility>
#include <iostream>
#include <fstream>

DirectedGraph::DirectedGraph(std::set<Vertex> vertices, std::set<DirectedEdge> edges)
    : vertices(vertices), edges(edges) {
    // make sure all edges are legal (parallels can't exist here because "edges" is a set)
    for (const DirectedEdge &edge : edges) {
        if (vertices.find(edge.getSource()) == vertices.end()) {
            throw IllegalEdge(edge, "Expecting source vertex to exist");
        }
        if (vertices.find(edge.getDestination()) == vertices.end()) {
            throw IllegalEdge(edge, "Expecting destination vertex to exist");
        }
        if (edge.getSource() == edge.getDestination()) {
            throw IllegalEdge(edge, "Self loop edges are not allowed");
        }
    }
}

DirectedGraph DirectedGraph::operator+(const DirectedGraph &graph) const {
    DirectedGraph unionGraph(graph);
    for (const Vertex& vertex : vertices) {
        unionGraph.vertices.insert(vertex);
    }
    for (const DirectedEdge& edge : edges) {
        unionGraph.edges.insert(edge);
    }
    return unionGraph;
}

DirectedGraph DirectedGraph::operator^(const DirectedGraph &graph) const {
    DirectedGraph intersectionGraph;
    for (const Vertex& vertex : vertices) {
        if (graph.vertices.find(vertex) != graph.vertices.end()) {
            intersectionGraph.vertices.insert(vertex);
        }
    }
    for (const DirectedEdge& edge : edges) {
        if (graph.edges.find(edge) != graph.edges.end()) {
            intersectionGraph.edges.insert(edge);
        }
    }
    return intersectionGraph;
}

DirectedGraph DirectedGraph::operator-(const DirectedGraph &graph) const {
    DirectedGraph differenceGraph;
    for (const Vertex& vertex : vertices) {
        if (graph.vertices.find(vertex) == graph.vertices.end()) {
            differenceGraph.vertices.insert(vertex);
        }
    }
    for (const DirectedEdge& edge : edges) {
        if (differenceGraph.vertices.find(edge.getSource()) != differenceGraph.vertices.end()
        && differenceGraph.vertices.find(edge.getDestination()) != differenceGraph.vertices.end()){
            differenceGraph.edges.insert(edge);
        }
    }
    return differenceGraph;
}

DirectedGraph DirectedGraph::operator*(const DirectedGraph &graph) const {
    DirectedGraph productGraph;
    for (const Vertex &vertex1 : vertices) {
        for (const Vertex &vertex2 : graph.vertices) {
            productGraph.vertices.insert(vertex1 + vertex2);
        }
    }
    for (const DirectedEdge &edge1 : edges) {
        for (const DirectedEdge &edge2 : graph.edges) {
            productGraph.edges.insert(DirectedEdge(edge1.getSource() + edge2.getSource(),
                    edge1.getDestination() + edge2.getDestination()));
        }
    }
    return productGraph;
}

DirectedGraph DirectedGraph::operator!() {
    DirectedGraph complementGraph(vertices);
    for (const Vertex &vertex1 : vertices) {
        for (const Vertex &vertex2 : vertices) {
            if (vertex1 != vertex2) {
                DirectedEdge edge(vertex1, vertex2);
                if (edges.find(edge) == edges.end()) {
                    complementGraph.edges.insert(edge);
                }
            }
        }
    }
    return complementGraph;
}

static void writeVertexToFile(const Vertex &vertex, std::ofstream &outFile) {
    unsigned int vertexNameSize = vertex.getName().size();
    outFile.write((const char*)&vertexNameSize, sizeof(unsigned int));
    outFile.write(vertex.getName().c_str(), vertexNameSize * sizeof(char));
}
static Vertex readVertexFromFile(std::ifstream &inFile) {
    unsigned int vertexNameSize;
    inFile.read((char*)&vertexNameSize, sizeof(unsigned int));
    char * vertexName = new char[vertexNameSize + 1];
    inFile.read(vertexName, vertexNameSize * sizeof(char));
    vertexName[vertexNameSize] = '\0';
    std::string vertexNameString = std::string(vertexName);
    delete[] vertexName;
    Vertex vertex(vertexNameString);
    return vertex;
}

void DirectedGraph::save(const std::string& fileName) {
    std::ofstream outFile(fileName, std::ios_base::binary);
    if (!outFile.good()) {
        throw UnableToPerformSave(fileName);
    }
    unsigned int numVertices = vertices.size();
    outFile.write((const char*)&numVertices, sizeof(unsigned int));
    unsigned int numEdges = edges.size();
    outFile.write((const char*)&numEdges, sizeof(unsigned int));
    for (const Vertex& vertex : vertices) {
        writeVertexToFile(vertex, outFile);
    }
    for (const DirectedEdge& edge : edges) {
        writeVertexToFile(edge.getSource(), outFile);
        writeVertexToFile(edge.getDestination(), outFile);
    }
}

DirectedGraph DirectedGraph::load(const std::string &fileName) {
    std::ifstream inFile(fileName,std::ios_base::binary);
    if (!inFile.good()) {
        throw FileDoesNotExist(fileName);
    }
    unsigned int numVertices;
    inFile.read((char*)&numVertices, sizeof(unsigned int));
    unsigned int numEdges;
    inFile.read((char*)&numEdges, sizeof(unsigned int));

    std::set<Vertex> vertices;
    for (unsigned int i = 0; i < numVertices; ++i) {
        Vertex vertex = readVertexFromFile(inFile);
        if (vertices.find(vertex) != vertices.end()) {
            // error can only be caught before adding vertex to set
            throw IllegalVertex(vertex, "Vertex name must be unique");
        }
        vertices.insert(vertex);
    }

    std::set<DirectedEdge> edges;
    for (unsigned int i = 0; i < numEdges; ++i) {
        Vertex source = readVertexFromFile( inFile);
        Vertex destination = readVertexFromFile(inFile);
        DirectedEdge edge(source, destination);
        if (edges.find(edge) != edges.end()) {
            // error can only be caught before adding edge to set
            throw IllegalEdge(edge, "Parallel edges are not allowed");
        }
        edges.insert(edge);
    }

    return DirectedGraph(vertices, edges);
}

void DirectedGraph::print(std::ostream &cout) const {
    for (const Vertex &vertex : vertices) {
        vertex.print(cout);
        cout << std::endl;
    }
    cout << "$" << std::endl;
    for (const DirectedEdge &edge : edges) {
        edge.print(cout);
        cout << std::endl;
    }
}

bool DirectedGraph::containsVertex(const Vertex &vertex) const {
    return vertices.find(vertex) != vertices.end();
}

bool DirectedGraph::containsEdge(const DirectedEdge &edge) const {
    return edges.find(edge) != edges.end();
}


