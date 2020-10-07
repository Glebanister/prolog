#pragma once

#include <deque>
#include <memory>
#include <ostream>
#include <stdexcept>

namespace prolog
{
struct Printable
{
    virtual void print(std::ostream &) const = 0;
    virtual ~Printable() noexcept = default;
};

namespace exception
{
class Exception : public Printable
{
public:
    explicit Exception(const std::string &name,
                       const std::string &message);

    const char *what() const noexcept;
    void print(std::ostream &) const override;

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
