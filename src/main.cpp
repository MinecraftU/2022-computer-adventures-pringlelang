#include "../inc/source_code.hpp"
#include "../inc/type.hpp"
#include "../inc/parser.hpp"

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
