#pragma once

#include <deque>
#include <memory>
#include <stdexcept>

namespace prolog
{
namespace exception
{
class Exception
{
public:
    explicit Exception(const std::string &name,
                       const std::string &message);

    const char *what() const noexcept;
    std::string reportStack() const noexcept;

    virtual ~Exception() noexcept = default;

    bool isEmpty() const noexcept;

    void push(std::shared_ptr<Exception>);
    void pop();
    bool isStackEmpty() const noexcept;

private:
    std::string msg_;

protected:
    bool empty_ = false;
    std::deque<std::shared_ptr<Exception>> stack_;
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

    const std::size_t line, linePos;
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
