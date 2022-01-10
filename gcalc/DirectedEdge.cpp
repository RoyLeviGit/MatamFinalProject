//
// Created by Roy Levi on 03/08/2020.
//

#include "DirectedEdge.h"

#include <utility>

DirectedEdge::DirectedEdge(const Vertex& source, const Vertex& destination) :
    source(source), destination(destination) {}

Vertex DirectedEdge::getSource() const {
    return source;
}

Vertex DirectedEdge::getDestination() const {
    return destination;
}

bool DirectedEdge::operator==(const DirectedEdge &edge) const {
    return source == edge.source && destination == edge.destination;
}

bool DirectedEdge::operator<(const DirectedEdge &edge) const {
    if (source == edge.source) {
        return destination < edge.destination;
    }
    return source < edge.source;
}

void DirectedEdge::print(std::ostream &cout) const {
    source.print(cout);
    cout << std::string(" ");
    destination.print(cout);
}

std::string DirectedEdge::toString() const {
    return "<" + source.getName() + ", " + destination.getName() + ">";
}

