#include "Expression.hpp"

#include <sstream>

namespace prolog
{
SyntaxTree::SyntaxTree(std::string label, std::string code)
    : label_(std::move(label) + ": '" + std::move(code) + "'") {}

void SyntaxTree::addSubtree(std::shared_ptr<SyntaxTree> tree)
{
    subtrees_.push_back(std::move(tree));
}

void SyntaxTree::resetSubtrees()
{
    subtrees_.clear();
}

void indentate(std::ostream &os, std::size_t indent, const std::string &indentationPattern)
{
    for (std::size_t i = 0; i < indent; ++i)
    {
        os << indentationPattern;
    }
}

void SyntaxTree::print(std::ostream &os) const
{
    struct Printer
    {
        const std::string indentation;
        const std::size_t indentationLevel;
        std::ostream &os;

        void print(const std::string &s)
        {
            indentate();
            os << s;
        }

        void printLn(const std::string &s)
        {
            print(s);
            endl();
        }

        void endl()
        {
            os << '\n';
        }

    private:
        void indentate()
        {
            for (std::size_t i = 0; i < indentationLevel; ++i)
            {
                os << indentation;
            }
        }
    } printer{"│  ", indentation_, os};

    printer.printLn("├─ " + label_);
    for (const auto &tree : subtrees_)
    {
        tree->indentation_ = indentation_ + 1;
        tree->print(os);
    }
}
} // namespace prolog
