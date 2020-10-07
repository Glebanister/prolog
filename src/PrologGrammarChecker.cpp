#include "PrologGrammarChecker.hpp"
#include "GrammarChecker.hpp"
#include "Lexer.hpp"

namespace prolog
{
namespace grammar
{
std::vector<std::vector<peach::token::TokenPtr>> splitByPeriod(std::vector<peach::token::TokenPtr> &&tokens)
{
    std::vector<std::vector<peach::token::TokenPtr>> declarations;

    std::vector<peach::token::TokenPtr> currentDeclaration;
    for (auto &&tok : tokens)
    {
        auto category = tok->getCategory();
        currentDeclaration.push_back(std::move(tok));
        if (category == prolog::lexer::prologTokens::PERIOD)
        {
            if (!currentDeclaration.empty())
            {
                declarations.push_back(std::move(currentDeclaration));
            }
            currentDeclaration.clear();
        }
    }
    if (!currentDeclaration.empty())
    {
        declarations.push_back(std::move(currentDeclaration));
    }

    return declarations;
}

std::vector<std::shared_ptr<exception::Exception>> checkPrologProgram(const std::string &programText)
{
    std::vector<std::shared_ptr<exception::Exception>> exceptions;

    prolog::lexer::PrologLexer lexer;
    std::vector<peach::token::TokenPtr> programTokens = lexer.tokenizeText(programText);
    for (const auto &tk : programTokens)
    {
        if (!tk->getCategory())
        {
            exceptions.push_back(std::make_shared<exception::TokenException>(tk->getLine(), tk->getLinePosition()));
        }
    }
    if (!exceptions.empty())
    {
        return exceptions;
    }

    for (auto &&tokens : splitByPeriod(std::move(programTokens)))
    {
        tokens.push_back(std::make_unique<peach::token::Token>(peach::token::tokenCategory::BRACKET_OPEN,
                                                               "begin",
                                                               0,
                                                               0,
                                                               0));
        std::reverse(tokens.begin(), tokens.end());
        tokens.push_back(std::make_unique<peach::token::Token>(peach::token::tokenCategory::BRACKET_OPEN,
                                                               "end",
                                                               std::count(programText.begin(), programText.end(), '\n'),
                                                               0,
                                                               programText.length()));
        std::reverse(tokens.begin(), tokens.end());

        using namespace prolog::grammar;
        using equals = GrammarUnitSequence;
        using peach::token::tokenCategory;
        using prolog::lexer::prologTokens;

        enum nonterm_t : prolog::grammar::grammarUnit_t
        {
            DISJ = prolog::grammar::TERMINAL_UNIT + 1,
            CONJ,
            NAME,
            DECL,
            BODY,
            ATOM,
            ATOM_SEQUENCE,
            ATOM_BRACKETS,
        };

        auto str = [&](std::string str, std::string desc) {
            return GrammarUnit{[=](const peach::token::Token &tk) { return tk.getTokenString() == str; },
                               std::move(desc) + " ('" + str + "')"};
        };
        auto oper = [&](std::string oper, std::string desc) {
            return GrammarUnit{[=](const peach::token::Token &tk) { return tk.getTokenString() == oper; },
                               "operator " + desc};
        };
        auto tok = [&](peach::token::tokenCategory_t cat, std::string desc) {
            return GrammarUnit{[=](const peach::token::Token &tk) { return tk.getCategory() == cat; },
                               "token of category '" + std::move(desc) + "'"};
        };

        // Terminals
        GrammarUnit id = tok(prologTokens::NAME, "identifier");
        GrammarUnit bOpen = str("(", "opening bracket");
        GrammarUnit bClose = str(")", "closing bracket");
        GrammarUnit corkscrew = str(":-", "corckscrew");
        GrammarUnit opOr = str(";", "semicolon");
        GrammarUnit opAnd = str(",", "comma");
        GrammarUnit period = str(".", "period");

        // Non-terminals
        GrammarUnit disj{DISJ, "disjunction"};
        GrammarUnit conj{CONJ, "conjunction"};
        GrammarUnit name{NAME, "indentificator"};
        GrammarUnit decl{DECL, "function declaration"};
        GrammarUnit body{BODY, "function body"};
        GrammarUnit atom{ATOM, "atom"};
        GrammarUnit atom_seq{ATOM_SEQUENCE, "atom sequence"};
        GrammarUnit atom_brackets{ATOM_BRACKETS, "atom in brackets"};

        std::vector<prolog::grammar::Rule> rules =
            {
                {atom, equals{id, atom_seq}},

                {atom_seq, equals{atom}},
                {atom_seq, equals{}},
                {atom_seq, equals{bOpen, atom_brackets, bClose, atom_seq}},

                {atom_brackets, equals{bOpen, atom_brackets, bClose}},
                {atom_brackets, equals{atom}},

                {decl, equals{atom, corkscrew, body, period}},
                {decl, equals{atom, period}},

                {body, equals{disj}},

                {disj, equals{conj, opOr, disj}},
                {disj, equals{conj}},

                {conj, equals{atom, opAnd, conj}},
                {conj, equals{bOpen, disj, bClose, opAnd, conj}},
                {conj, equals{bOpen, disj, bClose}},
                {conj, equals{atom}},
            };

        auto matchResult = prolog::grammar::matchTokensToGrammar(prolog::grammar::GrammarUnit{DECL,
                                                                                              tokens.begin() + 1,
                                                                                              tokens.end() - 1,
                                                                                              decl.description()},
                                                                 rules,
                                                                 {{"(", ")"}});
        if (!matchResult.exception->isEmpty())
        {
            exceptions.push_back(std::move(matchResult.exception));
        }
    }
    return exceptions;
}
} // namespace grammar
} // namespace prolog
