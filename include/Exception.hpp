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

    bool isEmpty() const noexcept;

    void extend(const Exception &);

private:
    std::string msg_;

protected:
    bool empty_ = false;
};

class EmptyException : public Exception
{
public:
    EmptyException()
        : Exception("EmptyException", "no exception occured")
    {
        empty_ = true;
    }
};

class PositionalException : public Exception
{
public:
    PositionalException(const std::string &name,
                        const std::string &message,
                        std::size_t line,
                        int linePos);
};

class TokenException : public PositionalException
{
public:
    TokenException(std::size_t line,
                   int linePos);
};

class GrammarException : public PositionalException
{
public:
    GrammarException(const std::string &message,
                     std::size_t line,
                     int linePos);
};
} // namespace exception
} // namespace prolog
