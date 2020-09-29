#include "GrammarChecker.hpp"

namespace prolog
{
namespace grammar
{

GrammarUnit::GrammarUnit(grammarUnitType type)
    : type(type) {}

GrammarUnit::GrammarUnit(grammarUnitType type, TokenPtrIterator begin, TokenPtrIterator end)
    : type(type),
      begin(begin),
      end(end)
{
}

GrammarUnit GrammarUnit::terminal()
{
    return {TERMINAL_UNIT};
}

GrammarUnit GrammarUnit::nonTerminal(grammarUnitType type, TokenPtrIterator begin, TokenPtrIterator end)
{
    if (type == TERMINAL_UNIT)
    {
        throw std::invalid_argument("nonterminal can not have grammarUnitType " + std::to_string(TERMINAL_UNIT));
    }
    return {type, begin, end};
}

bool GrammarUnit::isTerminal() const noexcept
{
    return type == TERMINAL_UNIT;
}
} // namespace grammar
} // namespace prolog
