#include "../inc/source_code.hpp"

std::string SourceCode::replace_args(std::vector<std::string> args) {
    if (args.size() != arg_names.size()) {
        std::cout << "Argument Error: Incorrect amount of aruments (expected " << arg_names.size() << " arguments, got " << args.size() << " arguments).";
    }

    std::string ret = raw;
    for (size_t i = 0; i < args.size(); i++) {
        // https://stackoverflow.com/a/3418285/16191068
        if (arg_names[i].empty()) continue;
        size_t start_pos = 0;
        while ((start_pos = ret.find(arg_names[i], start_pos)) != std::string::npos) {
            if (start_pos == 0 || (!isalnum(ret[start_pos-1]) && !isalnum(ret[start_pos + arg_names[i].length()]))) 
                ret.replace(start_pos, arg_names[i].length(), args[i]);
            start_pos += args[i].length();
        }
    }

    return ret;
}