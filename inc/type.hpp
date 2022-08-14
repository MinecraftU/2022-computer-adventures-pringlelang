#pragma once

#include "source_code.hpp"

enum Type {
    type_int = 0,
    type_string = 1,
};

struct Value {
    private:
    int type;
    int val_int;
    std::string val_string;

    public:
    Value() = default;

    Value(int val_int_in) {
        type = type_int;
        val_int = val_int_in;
    }

    Value(std::string val_string_in) {
        type = type_string;
        val_string = val_string_in;
    }

    Value(char val_string_in) { // char is coerced to string
        type = type_string;
        val_string = val_string_in;
    }

    int get_type();
    int get_int() const;
    std::string get_string() const;

    friend std::ostream& operator<<(std::ostream& os, const Value& v);

    std::string to_string() const;

    bool operator==(int i) const;
    bool operator==(std::string s) const;
};
