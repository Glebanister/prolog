#pragma once

#include <memory>
#include <ostream>
#include <string>
#include <vector>

#include "Exception.hpp"

namespace prolog
{
class SyntaxTree : public Printable
{
public:
    SyntaxTree(std::string label, std::string code);
    void addSubtree(std::shared_ptr<SyntaxTree>);
    void resetSubtrees();
    void print(std::ostream &os) const override;

private:
    std::vector<std::shared_ptr<SyntaxTree>> subtrees_;
    std::string label_;
    std::size_t indentation_ = 0;
};

template <typename... Args>
std::shared_ptr<SyntaxTree> makeTreePtr(Args &&... args)
{
    return std::make_shared<SyntaxTree>(std::forward<Args>(args)...);
}
} // namespace prolog
