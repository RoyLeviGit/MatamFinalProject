//
// Created by Roy Levi on 03/08/2020.
//

#ifndef MATAMFINALPROJECT_EXCEPTIONS_H
#define MATAMFINALPROJECT_EXCEPTIONS_H

#include <exception>
#include <string>
#include <utility>

class Exception : public std::exception {
    std::string str_err;
public:
    explicit Exception(const std::string& error = "") : str_err("Error: " + error) {}
    const char *what() const noexcept override {
        return str_err.c_str();
    }
};

#endif //MATAMFINALPROJECT_EXCEPTIONS_H
