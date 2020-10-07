#include "Exception.hpp"

#include <cassert>
#include <sstream>

namespace prolog
{
namespace exception
{
Exception::Exception(const std::string &name,
                     const std::string &message)
    : msg_(name + ": " + message) {}

void Exception::push(std::shared_ptr<Exception> message)
{
    stack_.push_back(message);
}

void Exception::pop()
{
    stack_.pop_back();
}

bool Exception::isStackEmpty() const noexcept
{
    return stack_.empty();
}

const char *Exception::what() const noexcept
{
    return msg_.c_str();
}

std::string Exception::reportStack() const noexcept
{
    std::stringstream message;
    for (std::size_t i = 0; i < stack_.size(); ++i)
    {
        message << stack_[i]->reportStack() << std::endl;
    }
    message << msg_;
    return message.str();
}

bool Exception::isEmpty() const noexcept { return empty_; }

PositionalException::PositionalException(const std::string &name,
                                         const std::string &message,
                                         std::size_t line,
                                         int linePos)
    : Exception(name,
                message +
                    " at " +
                    std::to_string(line + 1) + ":" +
                    std::to_string(linePos + 1)),
      line(line),
      linePos(linePos)
{
}

TokenException::TokenException(std::size_t line,
                               int linePos)
    : PositionalException("TokenException", "undefined token", line, linePos) {}

GrammarException::GrammarException(const std::string &message,
                                   std::size_t line,
                                   int linePos)
    : PositionalException("GrammarException", message, line, linePos) {}
} // namespace exception
} // namespace prolog
