#include "Exception.hpp"

namespace prolog
{
namespace exception
{
Exception::Exception(const std::string &name,
                     const std::string &message)
    : msg_(name + ": " + message) {}

const char *Exception::what() const noexcept { return msg_.c_str(); }

bool Exception::isEmpty() const noexcept { return empty_; }

PositionalException::PositionalException(const std::string &name,
                                         const std::string &message,
                                         std::size_t line,
                                         int linePos)
    : Exception(name,
                message +
                    " at " +
                    std::to_string(line) + ":" +
                    std::to_string(linePos))
{
}

GrammarException::GrammarException(const std::string &message,
                                   std::size_t line,
                                   int linePos)
    : PositionalException("GrammarException", message, line, linePos) {}
} // namespace exception
} // namespace prolog
