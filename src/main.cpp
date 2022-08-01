#include <fstream>
#include "sourcecode.hpp"
#include "token.hpp"
#include "parser.hpp"
#include "lexer.hpp"
#include "interpreter.hpp"

// Driver code
int main()
{
    std::ifstream t("../input.txt");
    if (!t.is_open())
        throw std::runtime_error("File not found in specified location");
    ;
    SourceCode source(t);

    Lexer *lex = new Lexer(source);

    Parser par(lex);
    ASTNode *root = par.parse();

    Interpreter inter(root);
    std::cout << "Result: " << inter.interpret() << std::endl;
    return 0;
}