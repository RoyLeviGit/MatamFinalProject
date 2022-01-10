//
// Created by Roy Levi on 03/08/2020.
//

#include "Vertex.h"

#include <utility>
#include <iostream>

static bool isLegalName(const std::string& name) {
    if (name.empty()) {
        return false;
    }
    int count = 0;
    for (char c : name) {
        switch (c) {
            case '[':
                count++;
                continue;
            case ']':
                if (count <= 0) {
                    return false;
                }
                count--;
                continue;
            case ';':
                if (count <= 0) {
                    return false;
                }
                continue;
            default:
                if (!isalnum(c)) {
                    return false;
                }
                continue;
        }
    }
    return count == 0;
}

Vertex::Vertex(const std::string& name) : name(name) {
    if (!isLegalName(name)) {
        throw IllegalVertex(name);
    }
}

std::string Vertex::getName() const {
    return name;
}

bool Vertex::operator==(const Vertex &vertex) const {
    return name == vertex.name;
}

bool Vertex::operator!=(const Vertex &vertex) const {
    return !(*this == vertex);
}

bool Vertex::operator<(const Vertex &vertex) const {
    return name < vertex.name;
}

Vertex Vertex::operator+(const Vertex& vertex) const {
    return Vertex("[" + name + ";" + vertex.name + "]");
}

void Vertex::print(std::ostream &cout) const {
    cout << name;
}
