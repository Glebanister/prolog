#pragma once

#include <memory>
#include <optional>
#include <stdexcept>
#include <unordered_map>

#include "Exception.hpp"
#include "Tokenization/Token.hpp"

namespace prolog
{
namespace grammar
{
using MaybeException = std::optional<exception::Exception>;
using peach::token::TokenPtr;
using TokenPtrIterator = std::vector<peach::token::TokenPtr>::iterator;

using grammarUnitType = int;
static constexpr grammarUnitType TERMINAL_UNIT = 0;

struct GrammarUnit
{
    static GrammarUnit terminal();
    static GrammarUnit nonTerminal(grammarUnitType type, TokenPtrIterator begin, TokenPtrIterator end);

    const grammarUnitType type;
    const TokenPtrIterator begin, end;
    bool isTerminal() const noexcept;

private:
    GrammarUnit(grammarUnitType type = TERMINAL_UNIT);
    GrammarUnit(grammarUnitType type, TokenPtrIterator begin, TokenPtrIterator end);
};

class GrammarUnitSequence
{
public:
    void append(GrammarUnit &&);
    void append(const GrammarUnit &);

    MaybeException matchTokenSequence(TokenPtrIterator begin, TokenPtrIterator end) const;

private:
    std::vector<std::unique_ptr<GrammarUnit>> to_;
};

MaybeException match(TokenPtrIterator begin, TokenPtrIterator end,
                     const std::unordered_map<grammarUnitType, GrammarUnitSequence> &rules);

} // namespace grammar
} // namespace prolog
