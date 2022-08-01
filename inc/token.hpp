#pragma once

#include <string>
#include <iostream>

class Token
{
public:
    enum Type
    {
        NUMBER, IDENTIFIER,
        MUL, DIV, ADD, SUB, UNARY_SUB,
        LPAREN, RPAREN,
        ASSIGN, T_UNKNOWN, T_EOF
    };

    static std::string getTypeName(Type type);

    Token(Type type, int value = 0, const std::string &id = "")
        : type(type), value(value), id(id) {}
    ~Token() {}
    
    // accessors
    Type getType() { return type; }
    int getValue() { return value; }
    std::string getId() { return id; }

    // mutators
    void setType(Type type) { this->type = type; }
    void setValue(int value) { this->value = value; }
    void setId(const std::string &id) { this->id = id; }

    friend std::ostream &operator<<(std::ostream &Str, Token const &token);

private:
    Type type;
    int value;      // for NUMBER
    std::string id; // for IDENTIFIER
};