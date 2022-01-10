//
// Created by Roy Levi on 03/08/2020.
//

#ifndef MATAMFINALPROJECT_DIRECTEDGRAPH_H
#define MATAMFINALPROJECT_DIRECTEDGRAPH_H

#include <set>
#include "Vertex.h"
#include "DirectedEdge.h"
#include "Exceptions.h"

class DirectedGraph {
    std::set<Vertex> vertices;
    std::set<DirectedEdge> edges;
public:
    explicit DirectedGraph(std::set<Vertex>  vertices = std::set<Vertex>(),
            std::set<DirectedEdge>  edges = std::set<DirectedEdge>());

    DirectedGraph operator+(const DirectedGraph& graph) const;
    DirectedGraph operator^(const DirectedGraph& graph) const;
    DirectedGraph operator-(const DirectedGraph& graph) const;
    DirectedGraph operator*(const DirectedGraph& graph) const;
    DirectedGraph operator!();
    void save(const std::string& fileName);
    static DirectedGraph load(const std::string& fileName);
    void print(std::ostream &cout) const;

    bool containsVertex(const Vertex &vertex) const;
    bool containsEdge(const DirectedEdge &edge) const;

    class IllegalVertex : public Exception {
    public:
        explicit IllegalVertex(const Vertex &vertex, const std::string &message) :
                Exception("Illegal vertex " + vertex.getName() + ". " + message) {}
    };
    class IllegalEdge : public Exception {
    public:
        explicit IllegalEdge(const DirectedEdge &edge, const std::string &message) :
            Exception("Illegal edge " + edge.toString() + ". " + message) {}
    };
    class UnableToPerformSave : public Exception {
    public:
        explicit UnableToPerformSave(const std::string &fileName) :
                Exception("Unable to perform save to file \'" + fileName + "\'") {}
    };
    class FileDoesNotExist : public Exception {
    public:
        explicit FileDoesNotExist(const std::string &fileName) :
                Exception("File \'" + fileName + "\' does not exist") {}
    };
};

#endif //MATAMFINALPROJECT_DIRECTEDGRAPH_H
