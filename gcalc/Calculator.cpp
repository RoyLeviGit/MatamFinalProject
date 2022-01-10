//
// Created by Roy Levi on 03/08/2020.
//

#include <iostream>
#include <vector>
#include "Calculator.h"

DirectedGraph& Calculator::getVariable(const std::string &name) {
    auto search = variables.find(name);
    if (search == variables.end()) {
        throw UndefinedVariable(name);
    }
    return search->second;
}

const DirectedGraph& Calculator::getVariable(const std::string &name) const {
    auto search = variables.find(name);
    if (search == variables.end()) {
        throw UndefinedVariable(name);
    }
    return search->second;
}

void Calculator::insertVariable(const std::string &name, DirectedGraph &graph) {
    variables[name] = graph;
}

void Calculator::printVariable(const std::string& name, std::ostream &cout) const {
    getVariable(name).print(cout);
}

void Calculator::deleteVariable(const std::string &name) {
    auto search = variables.find(name);
    if (search == variables.end()) {
        throw UndefinedVariable(name);
    }
    variables.erase(search);
}

void Calculator::who(std::ostream &cout) const {
    for (const auto &it : variables) {
        cout << it.first << std::endl;
    }
}

void Calculator::reset() {
    variables.clear();
}
