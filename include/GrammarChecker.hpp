#pragma once

#include <functional>
#include <memory>
#include <optional>
#include <stdexcept>
#include <unordered_map>
#include <variant>

#include "Exception.hpp"
#include "Tokenization/Token.hpp"

namespace prolog
{
namespace grammar
{
using peach::token::TokenPtr;
using TokenPtrIterator = std::vector<peach::token::TokenPtr>::iterator;

using grammarUnit_t = int;
static constexpr grammarUnit_t TERMINAL_UNIT = 0;
using TokenMatcherType = std::function<bool(const peach::token::Token &)>;

class GrammarUnit
{
public:
    GrammarUnit(const TokenMatcherType &token, const std::string &unitDesctiption);
    GrammarUnit(TokenMatcherType &&token, const std::string &unitDescription);
    GrammarUnit(grammarUnit_t type, const std::string &unitDescription);
    GrammarUnit(grammarUnit_t type, TokenPtrIterator begin, TokenPtrIterator end, const std::string &unitDescription);

    bool isTerminal() const noexcept;
    grammarUnit_t getUnitType() const noexcept;

    const TokenPtrIterator begin() const;
    const TokenPtrIterator end() const;
    TokenPtrIterator begin();
    TokenPtrIterator end();

    const TokenMatcherType &matcher() const;
    const std::string &description() const;

private:
    const grammarUnit_t type_;
    const TokenMatcherType tokenMatcher_;
    const TokenPtrIterator begin_, end_;
    const std::string unitDescription_;
};

using GrammarUnitSequence = std::vector<GrammarUnit>;

struct Rule
{
    GrammarUnit from;
    GrammarUnitSequence to;

    Rule(const GrammarUnit &from, const GrammarUnitSequence &to);
};

struct MathingResult
{
    TokenPtrIterator lastMatched;
    std::shared_ptr<exception::Exception> exception = std::make_shared<exception::EmptyException>();
};

MathingResult matchTokensToGrammar(const GrammarUnit &nonterminal,
                                   const std::vector<Rule> &rules,
                                   const std::unordered_map<std::string, std::string> &braces);
} // namespace grammar
} // namespace prolog
