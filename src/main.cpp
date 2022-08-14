#include "source_code.hpp"
#include "type.hpp"
#include "parser.hpp"

int main()
{
    std::ifstream t("../example.txt");

    std::string raw_src((std::istreambuf_iterator<char>(t)),
                        std::istreambuf_iterator<char>());
    SourceCode src = SourceCode(raw_src);

    Parser parser;
    parser.parse(src);

    t.close();
}
