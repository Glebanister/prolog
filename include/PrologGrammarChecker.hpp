#pragma once

#include <vector>

#include "Exception.hpp"

namespace prolog
{
namespace grammar
{
std::vector<exception::Exception> checkPrologProgram(const std::string &programText);
} // namespace grammar
} // namespace prolog
