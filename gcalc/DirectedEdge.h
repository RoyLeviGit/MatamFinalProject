//
// Created by Roy Levi on 03/08/2020.
//

#ifndef MATAMFINALPROJECT_DIRECTEDEDGE_H
#define MATAMFINALPROJECT_DIRECTEDEDGE_H


#include <string>
#include "Vertex.h"
#include "Exceptions.h"

class DirectedEdge {
    Vertex source;
    Vertex destination;
public:
    DirectedEdge(const Vertex& source, const Vertex& destination);
    Vertex getSource() const;
    Vertex getDestination() const;
    bool operator==(const DirectedEdge &edge) const;
    bool operator<(const DirectedEdge &edge) const;
    void print(std::ostream &cout) const;
    std::string toString() const;
};

#endif //MATAMFINALPROJECT_DIRECTEDEDGE_H
