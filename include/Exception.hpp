#pragma once

#include <stdexcept>

namespace prolog
{
namespace exception
{
class Exception : public std::exception
{
public:
    explicit Exception(const std::string &name,
                       const std::string &message);

    virtual const char *what() const noexcept;

    virtual ~Exception() noexcept = default;

private:
    std::string msg_;
};

class PositionalException : public Exception
{
public:
    PositionalException(const std::string &name,
                        const std::string &message,
                        std::size_t line,
                        std::size_t linePos);
};

class GrammarException : public PositionalException
{
public:
    GrammarException(const std::string &message,
                     std::size_t line,
                     std::size_t linePos);
};
} // namespace exception
} // namespace prolog
