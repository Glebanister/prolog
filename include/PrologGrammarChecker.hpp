#pragma once

#include <memory>
#include <vector>

#include "Exception.hpp"

namespace prolog
{
namespace grammar
{
std::vector<std::shared_ptr<exception::Exception>> checkPrologProgram(const std::string &programText);
} // namespace grammar
} // namespace prolog
