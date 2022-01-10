//
// Created by Roy Levi on 03/08/2020.
//

#ifndef MATAMFINALPROJECT_PARSER_H
#define MATAMFINALPROJECT_PARSER_H

#include <iostream>
#include <stack>
#include "Calculator.h"

class Parser {
    bool running;
    std::istream &cin;
    std::ostream &cout;
    Calculator &calculator;

    bool runCalculatorCommand(std::string &commandLine);

    bool runVariableCommand(std::string &commandLine);
    bool runPrintCommand(std::string &commandLine);
    bool runDeleteCommand(std::string &commandLine);
    bool runSaveCommand(std::string &commandLine);

    bool runAssignmentCommand(std::string &commandLine);

    void processClosingParenthesis(std::stack<DirectedGraph> &vStack, std::stack<char> &opStack);
    size_t processLoadVariable(std::string &expresion, size_t pos, std::stack<DirectedGraph>& vStack);
    size_t processVariableGraph(std::string &expresion, size_t pos, std::stack<DirectedGraph>& vStack);
    size_t processInputGraph(std::string &expresion, size_t pos, std::stack<DirectedGraph>& vStack);
    void processInputOperator(char op, std::stack<DirectedGraph>& vStack, std::stack<char>& opStack);

    bool operatorCausesEvaluation(char op, char prevOp);
    void executeOperation(std::stack<DirectedGraph>& vStack, std::stack<char>& opStack);
public:
    enum Mode {
        prompt,
        batch
    };
    enum Mode mode;

    Parser(std::istream &cin, std::ostream &cout, Calculator &calculator, enum Mode mode);
    void parse();
    void runCommandLine(std::string &commandLine);
    DirectedGraph evaluateExpresion(std::string &expresion);

    class UnrecognizedCommand : public Exception {
    public:
        explicit UnrecognizedCommand(const std::string& command) :
                Exception("Unrecognized command \'" + command +"\'") {}
    };
    class EmptyExpresion : public Exception {
    public:
        explicit EmptyExpresion() :
                Exception("Empty expresion") {}
    };
    class IllegalVariableName : public Exception {
    public:
        explicit IllegalVariableName(const std::string& variableName) :
                Exception("Illegal variable name \'" + variableName +"\'") {}
    };
    class IllegalFileName : public Exception {
    public:
        explicit IllegalFileName(const std::string& fileName) :
                Exception("Illegal file name \'" + fileName +"\'") {}
    };
    class IncorrectUsage : public Exception {
    public:
        explicit IncorrectUsage(const std::string& functionName, const std::string& correctUsage ) :
                Exception("Incorrect usage. Usage: "+ functionName + correctUsage) {}
    };
    class IllegalSyntax : public Exception {
    public:
        explicit IllegalSyntax(const std::string& current, const std::string& expecting) :
                Exception("Illegal syntax \'" + current +"\'. Expecting: " + expecting) {}
    };
    class IllegalOperator : public Exception {
    public:
        explicit IllegalOperator(const std::string& operatorName) :
                Exception("Illegal operator \'" + operatorName +"\'") {}
    };
    class IllegalUsageOfOperator : public Exception {
    public:
        explicit IllegalUsageOfOperator(const std::string& operatorName) :
                Exception("Illegal usage of operator \'" + operatorName +"\'") {}
    };
};
bool isLoad(std::string &expresion, size_t pos);
bool isLegalVariableName(const std::string& name);
bool isLegalFileName(const std::string& name);

#endif //MATAMFINALPROJECT_PARSER_H
