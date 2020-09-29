#pragma once

#include "Tokenization/Finders/LiteralFinder.hpp"
#include "Tokenization/Finders/NameFinder.hpp"
#include "Tokenization/Finders/NumberFinder.hpp"
#include "Tokenization/Finders/OperatorFinder.hpp"
#include "Tokenization/Finders/SingleCharFinder.hpp"
#include "Tokenization/FsmCollection.hpp"

namespace prolog
{
namespace lexer
{
struct prologTokens : private peach::token::tokenCategory
{
    static constexpr peach::token::tokenCategory_t
        COMMA = peach::token::tokenCategory::_TOKEN_TOTAL,
        PERIOD = peach::token::tokenCategory::_TOKEN_TOTAL + 1,
        MODULE = peach::token::tokenCategory::_TOKEN_TOTAL + 2,
        SIG = peach::token::tokenCategory::_TOKEN_TOTAL + 3,
        TYPE = peach::token::tokenCategory::_TOKEN_TOTAL + 4,
        LITERAL = peach::token::tokenCategory::_TOKEN_TOTAL + 5;
};

class PrologLexer : protected peach::fsm::FsmCollection
{
public:
    PrologLexer();
    std::vector<peach::token::TokenPtr> tokenizeText(const std::string &);

private:
    const std::vector<std::pair<std::string, peach::token::tokenCategory_t>> keywords_ = {
        {
            {"module", prologTokens::MODULE},
            {"sig", prologTokens::SIG},
            {"type", prologTokens::TYPE},
        }};
};
} // namespace lexer
} // namespace prolog
