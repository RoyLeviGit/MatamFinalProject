//
// Created by Roy Levi on 03/08/2020.
//

#ifndef MATAMFINALPROJECT_CALCULATOR_H
#define MATAMFINALPROJECT_CALCULATOR_H


#include <map>
#include "DirectedGraph.h"
#include "Exceptions.h"

class Calculator {
    std::map<std::string, DirectedGraph> variables;
public:
    DirectedGraph& getVariable(const std::string& name);
    const DirectedGraph &getVariable(const std::string &name) const;

    void insertVariable(const std::string& name, DirectedGraph& graph);
    void printVariable(const std::string& name, std::ostream &cout) const ;
    void deleteVariable(const std::string& name);
    void who(std::ostream &cout) const;
    void reset();

    class UndefinedVariable : public Exception {
    public:
        explicit UndefinedVariable(const std::string &variableName) :
            Exception("Undefined variable \'" + variableName +"\'") {}
    };
};

#endif //MATAMFINALPROJECT_CALCULATOR_H
