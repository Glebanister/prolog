#include "GrammarChecker.hpp"

#include <stack>

namespace prolog
{
namespace grammar
{
GrammarUnit::GrammarUnit(const TokenMatcherType &m, const std::string &unitDescription)
    : type_(TERMINAL_UNIT), tokenMatcher_(m), unitDescription_(unitDescription) {}

GrammarUnit::GrammarUnit(TokenMatcherType &&m, const std::string &unitDescription)
    : type_(TERMINAL_UNIT), tokenMatcher_(std::move(m)), unitDescription_(unitDescription) {}

GrammarUnit::GrammarUnit(grammarUnit_t type, const std::string &unitDescription)
    : type_(type), unitDescription_(unitDescription)
{
    if (!type)
    {
        throw std::invalid_argument("grammar unit type can not be 0");
    }
}

GrammarUnit::GrammarUnit(grammarUnit_t type, TokenPtrIterator begin, TokenPtrIterator end, const std::string &unitDescription)
    : type_(type), begin_(begin), end_(end), unitDescription_(unitDescription)
{
    if (!type)
    {
        throw std::invalid_argument("grammar unit type can not be 0");
    }
}

bool GrammarUnit::isTerminal() const noexcept { return type_ == TERMINAL_UNIT; }
grammarUnit_t GrammarUnit::getUnitType() const noexcept { return type_; }

const TokenPtrIterator GrammarUnit::begin() const { return begin_; }
const TokenPtrIterator GrammarUnit::end() const { return end_; }
TokenPtrIterator GrammarUnit::begin() { return begin_; }
TokenPtrIterator GrammarUnit::end() { return end_; }

const TokenMatcherType &GrammarUnit::matcher() const { return tokenMatcher_; }
const std::string &GrammarUnit::description() const { return unitDescription_; }

MathingResult matchTokensToGrammar(const GrammarUnit &nonterminal,
                                   const std::vector<Rule> &rules,
                                   const std::unordered_map<std::string, std::string> &braces)
{
    auto longestMatch = nonterminal.begin();
    bool hasOneMatch = false;
    exception::Exception bestMatchException = exception::EmptyException();

    auto updateBestMatch = [&](exception::Exception ex, TokenPtrIterator matched) {
        if (!hasOneMatch)
        {
            longestMatch = matched;
            bestMatchException = ex;
            hasOneMatch = true;
            return;
        }
        if (matched > longestMatch)
        {
            longestMatch = matched;
            bestMatchException = std::move(ex);
        }
    };

    for (const auto &rule : rules)
    {
        const auto &type = rule.from;
        const auto &seq = rule.to;
        TokenPtrIterator tokenIt = nonterminal.begin();
        if (type != nonterminal.getUnitType())
        {
            continue;
        }
        std::size_t unitId = 0;
        for (; unitId < seq.size(); ++unitId)
        {
            const auto &unit = seq[unitId];
            if (unit.isTerminal())
            {
                if (tokenIt >= nonterminal.end())
                {
                    auto badIter = std::prev(nonterminal.end());
                    auto except = exception::GrammarException{"expected " + unit.description(),
                                                              (*std::prev(nonterminal.end()))->getLine(),
                                                              (*std::prev(nonterminal.end()))->getLinePosition()};
                    updateBestMatch(std::move(except), nonterminal.end());
                    break;
                }
                else if (!unit.matcher()(**tokenIt))
                {
                    updateBestMatch(exception::GrammarException{"expected " + unit.description(),
                                                                (*tokenIt)->getLine(),
                                                                (*tokenIt)->getLinePosition()},
                                    tokenIt);
                    break;
                }
                ++tokenIt;
            }
            else
            {
                auto nextTerminal = tokenIt;
                if (unitId >= seq.size() - 1)
                {
                    nextTerminal = nonterminal.end();
                }
                else
                {
                    if (!seq[unitId + 1].isTerminal())
                    {
                        throw std::invalid_argument("two nonterminals in row are not allowed");
                    }
                    const auto &nextUnit = seq[unitId + 1];
                    std::stack<std::string> expectedBracetsStack;
                    for (; nextTerminal < nonterminal.end(); ++nextTerminal)
                    {
                        if (auto it = braces.find((*nextTerminal)->getTokenString()); it != braces.end())
                        {
                            expectedBracetsStack.emplace(it->second);
                            continue;
                        }
                        if (!expectedBracetsStack.empty() && expectedBracetsStack.top() == (*nextTerminal)->getTokenString())
                        {
                            expectedBracetsStack.pop();
                            continue;
                        }
                        if (!expectedBracetsStack.empty())
                        {
                            continue;
                        }
                        if (nextUnit.matcher()(**nextTerminal))
                        {
                            break;
                        }
                    }
                }
                if (nextTerminal >= nonterminal.end())
                {
                }
                auto matchingResult = matchTokensToGrammar(GrammarUnit{unit.getUnitType(), tokenIt, nextTerminal, unit.description()}, rules, braces);
                if (matchingResult.exception.isEmpty())
                {
                    tokenIt = nextTerminal;
                }
                else
                {
                    updateBestMatch(matchingResult.exception,
                                    matchingResult.lastMatched);
                    break;
                }
            }
        }
        if (tokenIt < nonterminal.end())
        {
            updateBestMatch(exception::GrammarException{"unexpected token sequence after token",
                                                        (*tokenIt)->getLine(),
                                                        (*tokenIt)->getLinePosition()},
                            tokenIt);
        }
        if (unitId < seq.size())
        {
            updateBestMatch(exception::GrammarException{"incomplete token sequence (is it " + nonterminal.description() + "?)",
                                                        (*std::prev(nonterminal.end()))->getLine(),
                                                        (*std::prev(nonterminal.end()))->getLinePosition()},
                            tokenIt);
        }
        if (tokenIt == nonterminal.end() && unitId == seq.size())
        {
            longestMatch = nonterminal.end();
            bestMatchException = exception::EmptyException();
            break;
        }
    }
    return {longestMatch, bestMatchException};
}
} // namespace grammar
} // namespace prolog
