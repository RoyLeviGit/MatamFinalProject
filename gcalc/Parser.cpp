//
// Created by Roy Levi on 03/08/2020.
//

#define RESERVED_WORD_PRINT "print"
#define RESERVED_WORD_PRINT_LENGTH 5
#define RESERVED_WORD_DELETE "delete"
#define RESERVED_WORD_DELETE_LENGTH 6
#define RESERVED_WORD_SAVE "save"
#define RESERVED_WORD_SAVE_LENGTH 4
#define RESERVED_WORD_LOAD "load"
#define RESERVED_WORD_LOAD_LENGTH 4
#define RESERVED_WORD_WHO "who"
#define RESERVED_WORD_RESET "reset"
#define RESERVED_WORD_QUIT "quit"

#include <stack>
#include "Parser.h"
#include "Utilities.h"

Parser::Parser(std::istream &cin, std::ostream &cout, Calculator &calculator, enum Mode mode)
    : running(false), cin(cin), cout(cout), calculator(calculator), mode(mode) {}

void Parser::parse() {
    running = true;
    while(running && !cin.eof()) {
        if (mode == prompt) {
            cout << "Gcalc> ";
        }
        std::string commandLine;
        std::getline(cin, commandLine);
        try {
            runCommandLine(commandLine);
        } catch (Exception &e) {
            cout << e.what() << std::endl;
        }
    }
}

void Parser::runCommandLine(std::string &commandLine) {
    commandLine = trim(commandLine);
    if (commandLine.empty()) {
        return;
    }
    if (runCalculatorCommand(commandLine) ||
        runVariableCommand(commandLine) ||
        runAssignmentCommand(commandLine)) {
        return;
    }
    throw UnrecognizedCommand(commandLine);
}

bool Parser::runCalculatorCommand(std::string &commandLine) {
    if (commandLine == RESERVED_WORD_QUIT) {
        running = false;
        return true;
    }
    if (commandLine == RESERVED_WORD_RESET) {
        calculator.reset();
        return true;
    }
    if (commandLine == RESERVED_WORD_WHO) {
        calculator.who(cout);
        return true;
    }
    return false;
}

bool Parser::runVariableCommand(std::string &commandLine) {
    if (runPrintCommand(commandLine) ||
        runDeleteCommand(commandLine) ||
        runSaveCommand(commandLine)) {
        return true;
    }
    return false;
}

bool Parser::runPrintCommand(std::string &commandLine) {
    if (commandLine.substr(0, RESERVED_WORD_PRINT_LENGTH) != RESERVED_WORD_PRINT) {
        return false;
    }
    commandLine = trim(commandLine.substr(RESERVED_WORD_PRINT_LENGTH,
                                          commandLine.length() - RESERVED_WORD_PRINT_LENGTH));
    if (commandLine[0] != '(' || commandLine[commandLine.size() - 1] != ')') {
        throw IncorrectUsage(RESERVED_WORD_PRINT, "(expresion)");
    }
    commandLine = trim(commandLine.substr(1, commandLine.length() - 2)); // commandLine is now expresion
    evaluateExpresion(commandLine).print(cout); // throws
    return true;
}

bool Parser::runDeleteCommand(std::string &commandLine) {
    if (commandLine.substr(0, RESERVED_WORD_DELETE_LENGTH) != RESERVED_WORD_DELETE) {
        return false;
    }
    commandLine = trim(commandLine.substr(RESERVED_WORD_DELETE_LENGTH,
                                          commandLine.length() - RESERVED_WORD_DELETE_LENGTH));
    if (commandLine[0] != '(' || commandLine[commandLine.size() - 1] != ')') {
        throw IncorrectUsage(RESERVED_WORD_DELETE, "(variable)");
    }
    commandLine = trim(commandLine.substr(1, commandLine.length() - 2)); // commandLine is now variable name
    calculator.deleteVariable(commandLine); // throws if undefined variable
    return true;
}

bool Parser::runSaveCommand(std::string &commandLine) {
    if (commandLine.substr(0, RESERVED_WORD_SAVE_LENGTH) != RESERVED_WORD_SAVE) {
        return false;
    }
    commandLine = trim(commandLine.substr(RESERVED_WORD_SAVE_LENGTH , commandLine.length() - RESERVED_WORD_SAVE_LENGTH));
    if (commandLine[0] != '(' || commandLine[commandLine.size() - 1] != ')') {
        throw IncorrectUsage(RESERVED_WORD_SAVE, "(expresion, filename)");
    }
    commandLine = trim(commandLine.substr(1, commandLine.length() - 2)); // commandLine is now without ()
    size_t searchForComma = commandLine.find_last_of(',');
    if (searchForComma == std::string::npos) {
        throw IncorrectUsage(RESERVED_WORD_SAVE, "(expresion, filename)");
    }
    std::string expresion = trim(commandLine.substr(0, searchForComma));
    std::string fileName = trim(commandLine.substr(searchForComma + 1));
    if (!isLegalFileName(fileName)) {
        throw IllegalFileName(fileName);
    }
    evaluateExpresion(expresion).save(fileName); // throws
    return true;
}

bool Parser::runAssignmentCommand(std::string &commandLine) {
    auto search = commandLine.find('=');
    if (search == std::string::npos) {
        return false;
    }
    std::string variableName = trim(commandLine.substr(0, search));
    if (!isLegalVariableName(variableName)) {
        throw IllegalVariableName(variableName);
    }
    std::string expresion = trim(commandLine.substr(search + 1, commandLine.length() - search));
    DirectedGraph evaluatedGraph = evaluateExpresion(expresion); // throws
    calculator.insertVariable(variableName, evaluatedGraph);
    return true;
}

DirectedGraph Parser::evaluateExpresion(std::string &expresion) {
    std::stack<DirectedGraph> vStack;
    std::stack<char> opStack;

    if (!isLegalParenthesis(expresion)) {
        throw IllegalSyntax(expresion, "valid usage of parenthesis");
    }

    opStack.push('(');

    bool expectingBinaryOperator = false;

    size_t pos = 0;
    while (pos <= expresion.length()) {
        if (opStack.empty()) {
            throw Exception("Invalid expresion"); // shouldn't get here
        }
        if (expresion[pos] == ' ') {
            pos++;
        }
        else if (pos == expresion.length() || expresion[pos] == ')') {
            processClosingParenthesis(vStack, opStack);
            pos++;
            expectingBinaryOperator = true;
        }
        else if (isalpha(expresion[pos])) {
            if (expectingBinaryOperator) {
                throw IllegalSyntax(expresion, "binary operator between variables");
            }
            if (isLoad(expresion, pos)) {
                pos = processLoadVariable(expresion, pos, vStack);
            } else {
                pos = processVariableGraph(expresion, pos, vStack);
            }
            expectingBinaryOperator = true;
        }
        else if (isdigit(expresion[pos])) {
            // variable starts with a number, therefore illegal variable name
            int endOfVariablePos = expresion.find_first_of(" +^-*!", pos);
            std::string variableName = expresion.substr(pos, endOfVariablePos - pos);
            throw IllegalVariableName(variableName);
        }
        else if (expresion[pos] == '{') {
            if (expectingBinaryOperator) {
                throw IllegalSyntax(expresion, "binary operator between variables");
            }
            pos = processInputGraph(expresion, pos, vStack);
            expectingBinaryOperator = true;
        }
        else if (expresion[pos] == '!') {
            if (pos+1 >= expresion.length() ||
                expresion.find_first_of("+^-*)", pos + 1) < expresion.find_first_not_of(" +^-*)", pos + 1)) { // second has ' '
                throw IllegalUsageOfOperator(std::string(1, '!'));
            }
            processInputOperator(expresion[pos], vStack, opStack);
            pos++;
        }
        else if (expresion[pos] == '(') {
            processInputOperator(expresion[pos], vStack, opStack);
            pos++;
        }
        else {
            // Binary operator
            if (!expectingBinaryOperator) {
                throw IllegalSyntax(expresion, "binary operator between variables");
            }
            processInputOperator(expresion[pos], vStack, opStack);
            pos++;
            expectingBinaryOperator = false;
        }
    }

    if (vStack.empty()) {
        throw EmptyExpresion();
    }
    if (!opStack.empty()) {
        throw Exception("Invalid expresion"); // shouldn't get here
    }
    if (vStack.size() > 1) {
        if (opStack.empty()) {
            throw IllegalSyntax(expresion, "more operators");
        }
    }
    return vStack.top();
}

void Parser::processClosingParenthesis(std::stack<DirectedGraph>& vStack, std::stack<char>& opStack) {
    if (opStack.empty()) {
        throw Exception("Invalid expresion"); // shouldn't get here
    }
    while (opStack.top() != '(') {
        executeOperation(vStack, opStack);
    }
    if (opStack.empty()) {
        throw Exception("Invalid expresion"); // shouldn't get here
    }
    opStack.pop(); // Remove the opening parenthesis
}

size_t Parser::processLoadVariable(std::string &expresion, size_t pos, std::stack<DirectedGraph>& vStack) {
    size_t endOfLoadPos = expresion.find(')', pos);
    if (endOfLoadPos == std::string::npos) {
        throw IncorrectUsage(RESERVED_WORD_LOAD, "(filename)");
    }
    std::string fileName = trim(expresion.substr(pos + RESERVED_WORD_LOAD_LENGTH , endOfLoadPos - pos - RESERVED_WORD_LOAD_LENGTH + 1));
    if (fileName[0] != '(' || fileName[fileName.size() - 1] != ')') {
        throw IncorrectUsage(RESERVED_WORD_LOAD, "(filename)");
    }
    fileName = trim(fileName.substr(1, fileName.length() - 2)); // fileName is now without ()
    if (!isLegalFileName(fileName)) {
        throw IllegalFileName(fileName);
    }
    DirectedGraph loadedGraph = DirectedGraph::load(fileName); // throws
    vStack.push(loadedGraph);

    return endOfLoadPos + 1;
}

size_t Parser::processVariableGraph(std::string &expresion, size_t pos, std::stack<DirectedGraph>& vStack) {
    size_t endOfVariablePos = expresion.find_first_of(" +^-*!)", pos);
    std::string variableName = endOfVariablePos == std::string::npos ? expresion.substr(pos) :
            expresion.substr(pos, endOfVariablePos - pos); // trim is redundant

    DirectedGraph evaluatedGraph = calculator.getVariable(variableName); // throws if undefined variable
    vStack.push(evaluatedGraph);

    return endOfVariablePos == std::string::npos ? static_cast<int>(expresion.length()) : endOfVariablePos;
}

size_t Parser::processInputGraph(std::string &expresion, size_t pos, std::stack<DirectedGraph>& vStack) {
    size_t endOfGraphPos = expresion.find('}', pos);
    if (endOfGraphPos == std::string::npos) {
        throw IllegalSyntax(expresion.substr(pos), "{v1, v2, ... |<v1,v2>, <v2,v1>, ... }");
    }
    std::string graphExpresion = trim(expresion.substr(pos + 1, endOfGraphPos - pos - 1));

    std::vector<std::string> verticesAndEdges = splitString(graphExpresion, '|');
    std::vector<std::string> verticesString = !verticesAndEdges.empty() ?
            splitString(verticesAndEdges[0], ',') : std::vector<std::string>();
    std::vector<std::string> edgesString = verticesAndEdges.size() > 1 ?
            splitString(connectString(splitString(verticesAndEdges[1], ',')), ">,<") :
            std::vector<std::string>();

    // Vertices
    std::set<Vertex> vertices;
    for (const std::string &vertexString : verticesString) {
        Vertex vertex(vertexString); // throws illegal vertex;
        if (vertices.find(vertex) != vertices.end()) {
            // error can only be caught before adding vertex to set
            throw DirectedGraph::IllegalVertex(vertex, "Vertex name must be unique");
        }
        vertices.insert(vertex);
    }

    // Edges
    std::set<DirectedEdge> edges;
    for (std::string edgeString : edgesString) {
        if (edgeString[0] != '<' || edgeString[edgeString.size()-1] != '>') {
            throw IllegalSyntax(edgeString, "<v1, v2>");
        }
        std::vector<std::string> sourceAndDestString = splitString(
                edgeString.substr(1, edgeString.length() - 2), ',');// remove '<', '>' and split
        if (sourceAndDestString.size() != 2) {
            throw IllegalSyntax(edgeString, "<v1, v2>");
        }

        Vertex source(sourceAndDestString[0]); // throws
        Vertex destination(sourceAndDestString[1]); // throws
        DirectedEdge edge(source, destination);
        if (edges.find(edge) != edges.end()) {
            // error can only be caught before adding edge to set
            throw DirectedGraph::IllegalEdge(edge, "Parallel edges are not allowed");
        }
        edges.insert(edge);
    }
    DirectedGraph graph(vertices, edges); // throws
    vStack.push(graph);

    return endOfGraphPos + 1;

}

void Parser::processInputOperator(char op, std::stack<DirectedGraph>& vStack, std::stack<char>& opStack) {


    while (!opStack.empty() && operatorCausesEvaluation(op, opStack.top())) {
        executeOperation(vStack, opStack);
    }

    opStack.push(op);

}

bool Parser::operatorCausesEvaluation(char op, char prevOp) {

    bool evaluate = false;

    switch (op)
    {
        case '+':
        case '-':
        case '*':
        case '^':
            evaluate = (prevOp != '(');
            break;
        case ')':
            evaluate = true;
            break;
        case '(':
        case '!':
            break;
        default:
            throw IllegalOperator(std::string(1, op)); // shouldn't get here
    }

    return evaluate;

}

void Parser::executeOperation(std::stack<DirectedGraph>& vStack, std::stack<char>& opStack) {
    if (opStack.empty()) {
        throw Exception("Invalid expresion"); // shouldn't get here
    }
    char op = opStack.top();
    opStack.pop();
    if (vStack.empty()) {
        throw IllegalUsageOfOperator(std::string(1, op));
    }
    DirectedGraph rightOperand = vStack.top(), leftOperand;
    vStack.pop();

    switch (op) {
        case '+':
        case '-':
        case '*':
        case '^':
            if (vStack.empty()) {
                throw IllegalUsageOfOperator(std::string(1, op));
            }
            leftOperand = vStack.top();
            vStack.pop();
            break;
        default:
            break;
    }

    DirectedGraph result;
    switch (op) {
        case '!':
            result = !rightOperand;
            break;
        case '+':
            result = leftOperand + rightOperand;
            break;
        case '-':
            result = leftOperand - rightOperand;
            break;
        case '*':
            result = leftOperand * rightOperand;
            break;
        case '^':
            result = leftOperand ^ rightOperand;
            break;
        case '(':
        case ')':
            break;
        default:
            throw Exception("Invalid expresion"); // shouldn't get here
    }

    vStack.push(result);
}

bool isLoad(std::string &expresion, size_t pos) {
    size_t endOfLoadPos = expresion.find_first_of(" +^-*!)(", pos); // search has (
    if (endOfLoadPos == std::string::npos) {
        return false;
    }
    std::string loadOrVariableName = expresion.substr(pos, endOfLoadPos - pos);

    if (loadOrVariableName != RESERVED_WORD_LOAD) {
        return false;
    }
    return expresion[pos + RESERVED_WORD_LOAD_LENGTH] == ' ' || expresion[pos + RESERVED_WORD_LOAD_LENGTH] == '(';
}

bool isLegalVariableName(const std::string& name) {
    if (!isalpha(name[0])) {
        return false;
    }
    for (char c : name) {
        if (!isalnum(c))
            return false;
    }
    const std::set<std::string> reservedWords = { RESERVED_WORD_PRINT, RESERVED_WORD_DELETE, RESERVED_WORD_SAVE, RESERVED_WORD_LOAD,
                                                  RESERVED_WORD_WHO, RESERVED_WORD_RESET, RESERVED_WORD_QUIT };
    return reservedWords.find(name) == reservedWords.end();
}

bool isLegalFileName(const std::string& name) {
    return (trim(name) == name) && (name.find_first_of("(),") == std::string::npos);
}