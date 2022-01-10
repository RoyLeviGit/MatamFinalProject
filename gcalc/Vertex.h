//
// Created by Roy Levi on 03/08/2020.
//

#ifndef MATAMFINALPROJECT_VERTEX_H
#define MATAMFINALPROJECT_VERTEX_H

#include <string>
#include <set>
#include "Exceptions.h"

class Vertex {
    std::string name;
public:
    explicit Vertex(const std::string& id);
    std::string getName() const;
    bool operator==(const Vertex &vertex) const;
    bool operator!=(const Vertex &vertex) const;
    bool operator<(const Vertex &vertex) const;
    Vertex operator+(const Vertex& vertex) const;
    void print(std::ostream &cout) const;

    class IllegalVertex : public Exception {
    public:
        explicit IllegalVertex(const std::string &vertexName) :
            Exception("Illegal vertex \'" + vertexName +"\'") {}
    };
};

#endif //MATAMFINALPROJECT_VERTEX_H
