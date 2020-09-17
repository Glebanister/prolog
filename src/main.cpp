#include <fstream>

#include "Tokenization/Finders/NameFinder.hpp"
#include "Tokenization/Finders/NumberFinder.hpp"
#include "Tokenization/Finders/OperatorFinder.hpp"
#include "Tokenization/Finders/SingleCharFinder.hpp"
#include "Tokenization/FsmCollection.hpp"

struct customTokens : private peach::token::tokenCategory
{
    static constexpr peach::token::tokenCategory_t
        COMMA = peach::token::tokenCategory::_TOKEN_TOTAL,
        DOT = peach::token::tokenCategory::_TOKEN_TOTAL + 1,
        MODULE = peach::token::tokenCategory::_TOKEN_TOTAL + 2,
        SIG = peach::token::tokenCategory::_TOKEN_TOTAL + 3,
        TYPE = peach::token::tokenCategory::_TOKEN_TOTAL + 4;
};

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

        using peach::token::tokenCategory;
        using peach::token::tokenCategory_t;

        peach::token::registerTokenCategoryString("COMMA");
        peach::token::registerTokenCategoryString("DOT");
        peach::token::registerTokenCategoryString("MODULE");
        peach::token::registerTokenCategoryString("SIG");
        peach::token::registerTokenCategoryString("TYPE");

        peach::fsm::FsmCollection lexer;
        lexer
            .buildAppendFsm<peach::fsm::NameFinder>()
            .buildAppendFsm<peach::fsm::NumberFinder>(tokenCategory::VALUE_INT)
            .buildAppendFsm<peach::fsm::OperatorFinder>(
                std::vector<std::pair<std::string, tokenCategory_t>>{
                    {":-", tokenCategory::ASSIGNMENT},
                    {"->", tokenCategory::OPERATOR_BI},
                }) //
            .buildAppendFsm<peach::fsm::SingleCharFinder>(
                std::vector<std::pair<char, tokenCategory_t>>{
                    {'\n', tokenCategory::SEP_ENDL},
                    {' ', tokenCategory::SEP_SPACE},
                    {'\t', tokenCategory::SEP_TAB},
                    {'[', tokenCategory::BRACKET_OPEN},
                    {']', tokenCategory::BRACKET_CLOSE},
                    {'.', customTokens::DOT},
                    {',', customTokens::COMMA},
                }) //
            ;

        std::string programText((std::istreambuf_iterator<char>(file)),
                                std::istreambuf_iterator<char>());
        auto tokens = lexer.tokenizeText(programText, {
                                                          {"module", customTokens::MODULE},
                                                          {"sig", customTokens::SIG},
                                                          {"type", customTokens::TYPE},
                                                      });

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
            peach::token::printCategory(ofs, token->getCategory());
            ofs << ' ';
            ofs << "'" << token->getTokenString() << "' " << token->getLine() << ' ' << token->getTextPosition() << '\n';
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}
