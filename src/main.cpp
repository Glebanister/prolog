#include <fstream>

#include "GrammarChecker.hpp"
#include "Lexer.hpp"

int main(int argc, char **argv)
{
    try
    {
        std::vector<peach::token::TokenPtr> tokens;

        std::string programText = "1 + 1 + 1";

        prolog::lexer::PrologLexer lexer;
        tokens = lexer.tokenizeText(programText);

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

        auto str = [](std::string s) {
            return GrammarUnit([=](const peach::token::Token &tk) { return tk.getTokenString() == s; }, "string '" + s + "'");
        };

        using seq = GrammarUnitSequence;
        GrammarUnit A{1, "A"};

        enum nonterm_t : prolog::grammar::grammarUnit_t
        {
            TERM = 1,
            FACTOR,
            NUM,
        };

        GrammarUnit term{TERM, "summation"};
        GrammarUnit fact{FACTOR, "multiplication"};
        GrammarUnit num{NUM, "integer"};

        auto oper = [&](std::string op) {
            return GrammarUnit{[=](const peach::token::Token &tk) { return tk.getTokenString() == op; },
                               "operator"};
        };

        auto tok = [&](peach::token::tokenCategory_t cat) {
            return GrammarUnit{[=](const peach::token::Token &tk) { return tk.getCategory() == cat; },
                               "token of category '" + peach::token::tokenCategoryString[cat] + "'"};
        };

        std::vector<prolog::grammar::Rule> rules =
            {
                {TERM, seq{num, oper("+"), term}},
                {TERM, seq{num, oper("-"), term}},
                {TERM, seq{num}},
                {NUM, seq{tok(peach::token::tokenCategory::VALUE_INT)}},
            };

        auto matchResult = prolog::grammar::matchTokensToGrammar(prolog::grammar::GrammarUnit{TERM, tokens.begin() + 1, tokens.end() - 1, "summation"},
                                                                 rules,
                                                                 {{"(", ")"}});

        if (!matchResult.exception.isEmpty())
        {
            std::cout << matchResult.exception.what() << std::endl;
        }
        else
        {
            std::cout << "correct" << std::endl;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return 1;
    }
}
