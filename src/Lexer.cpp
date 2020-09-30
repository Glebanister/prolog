#include "Lexer.hpp"

namespace prolog
{
namespace lexer
{
PrologLexer::PrologLexer()
{
    using peach::token::tokenCategory;
    using peach::token::tokenCategory_t;
    buildAppendFsm<peach::fsm::NameFinder>()
        .buildAppendFsm<peach::fsm::NumberFinder>(tokenCategory::VALUE_INT)
        .buildAppendFsm<peach::fsm::LiteralFinder<'"'>>(prologTokens::LITERAL)
        .buildAppendFsm<peach::fsm::OperatorFinder>(
            std::vector<std::pair<std::string, tokenCategory_t>>{
                {":-", prologTokens::CORKSCREW},
                {"->", tokenCategory::OPERATOR_BI},
            })
        .buildAppendFsm<peach::fsm::SingleCharFinder>(
            std::vector<std::pair<char, tokenCategory_t>>{
                {'\n', tokenCategory::SEP_ENDL},
                {' ', tokenCategory::SEP_SPACE},
                {'\t', tokenCategory::SEP_TAB},
                {'[', tokenCategory::BRACKET_OPEN},
                {']', tokenCategory::BRACKET_CLOSE},
                {'(', tokenCategory::BRACKET_OPEN},
                {')', tokenCategory::BRACKET_CLOSE},
                {'.', prologTokens::PERIOD},
                {',', prologTokens::COMMA},
                {';', prologTokens::SEMICOLON},
            });
}

std::vector<peach::token::TokenPtr> PrologLexer::tokenizeText(const std::string &text)
{
    std::vector<peach::token::TokenPtr> tks;
    for (auto &&tk : peach::fsm::FsmCollection::tokenizeText(text, keywords_))
    {
        if (!peach::token::isSeparator(tk))
        {
            tks.emplace_back(std::move(tk));
        }
    }
    return tks;
}
} // namespace lexer
} // namespace prolog
