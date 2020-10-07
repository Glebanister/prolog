#pragma once

#include <memory>
#include <variant>
#include <vector>

#include "Exception.hpp"
#include "Expression.hpp"

namespace prolog
{
namespace grammar
{
std::vector<std::shared_ptr<Printable>> checkPrologProgram(const std::string &programText);
} // namespace grammar
} // namespace prolog
