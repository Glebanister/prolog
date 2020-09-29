#include <fstream>

#include "Lexer.hpp"

int main(int argc, char **argv)
{
    try
    {
        if (argc < 2)
        {
            throw std::invalid_argument("specify input file name as first argument");
        }
        std::ifstream file(argv[1]);
        if (!file.good())
        {
            throw std::invalid_argument("unable to read '" + std::string(argv[1]) + "'");
        }

        std::string programText((std::istreambuf_iterator<char>(file)),
                                std::istreambuf_iterator<char>());

        prolog::lexer::PrologLexer lexer;
        auto tokens = lexer.tokenizeText(programText);

        std::ofstream ofs(std::string(argv[1]) + ".sig");

        if (!ofs.good())
        {
            throw std::invalid_argument("unable to write to '" + std::string(argv[1]) + ".sig" + "'");
        }

        for (auto &token : tokens)
        {
            if (peach::token::isSeparator(token))
            {
                continue;
            }
            ofs << ' ';
            ofs << "'" << token->getTokenString() << "' " << token->getLine() << ' ' << token->getTextPosition() << '\n';
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return 1;
    }
}
