//
// Created by Roy Levi on 04/08/2020.
//

#ifndef MATAMFINALPROJECT_UTILITIES_H
#define MATAMFINALPROJECT_UTILITIES_H

#define WHITESPACE " \n\r\t\f\v"

#include <string>
#include <vector>

static std::string ltrim(const std::string& s)
{
    size_t start = s.find_first_not_of(WHITESPACE);
    return (start == std::string::npos) ? "" : s.substr(start);
}

static std::string rtrim(const std::string& s)
{
    size_t end = s.find_last_not_of(WHITESPACE);
    return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}

std::string trim(const std::string& s)
{
    return rtrim(ltrim(s));
}

std::vector<std::string> splitString(const std::string& sentence, char by) {
    std::string word;
    std::vector<std::string> splitSentence;
    if (sentence.empty()) {
        return splitSentence;
    }

    for (auto c : sentence) {
        if (c == by) {
            splitSentence.push_back(trim(word)); // trims words before adding them
            word = "";
        } else {
            word += c;
        }
    }
    splitSentence.push_back(trim(word));
    return splitSentence;
}

std::vector<std::string> splitString(const std::string& sentence, const std::string& by) {
    std::vector<std::string> splitSentence;
    size_t last = -1;
    size_t next = sentence.find(by);
    if (sentence.empty()) { // acting weird if no >,< are found
        return splitSentence;
    }

    while (next != std::string::npos) {
        splitSentence.push_back(trim(sentence.substr(last + 1, next-last)));
        last = next + 1;
        next = sentence.find(by, last);
    }

    std::string word = sentence.substr(last + 1);
    splitSentence.push_back(trim(word));
    return splitSentence;
}

std::string connectString(const std::vector<std::string>& brokenString) {
    std::string sentence;
    for (const std::string &word : brokenString) {
        sentence += word + ",";
    }
    if (!sentence.empty()) {
        sentence = sentence.substr(0,sentence.length() - 1);
    }
    return sentence;
}

bool isLegalParenthesis(const std::string &expresion) {
    int count = 0;
    bool parenthesisEmpty = false;
    for (char c : expresion) {
        switch (c) {
            case ' ':
                continue;
            case '(':
                parenthesisEmpty = true;
                ++count;
                continue;
            case ')':
                if (parenthesisEmpty || count < 0) {
                    return false;
                }
                --count;
                continue;
            default:
                parenthesisEmpty = false;
                continue;
        }
    }
    return count == 0;
}

#endif //MATAMFINALPROJECT_UTILITIES_H
