#include "type.hpp"

int Value::get_type() {
    return type;
}

int Value::get_int() const {
    if (type == type_int) {
        return val_int;
    } else {
        std::cout << "Argument Error: incorrect argument type (did not get string)!";
        exit(1);
    }
}

std::string Value::get_string() const {
    if (type == type_string) {
        return val_string;
    } else {
        std::cout << "Argument Error: incorrect argument type (did not get int)!";
        exit(1);
    }
}

std::string Value::to_string() const {
    if (type == type_int) {
        return std::to_string(val_int);
    } else if (type == type_string) {
        return val_string;
    } else {
        std::cout << "Argument Error: argument is of illegal type";
        exit(1);
    }
}

std::ostream& operator<<(std::ostream& os, const Value& v) {
    os << v.to_string();
    return os;
}

bool Value::operator==(int i) const {
    return get_int() == i;
}

bool Value::operator==(std::string s) const {
    return get_string() == s;
}
