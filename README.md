# Prolog grammar checker

Formal languages HW №4.

## Installation

```bash
git clone https://github.com/Glebanister/prolog --recursive
cd prolog
mkdir build && cd build
cmake ..
make
```

## Usage

```bash
./prolog-checker [OPTION...]
```

| Option        | Meaning              | Value  |
|---------------|----------------------|--------|
| `-i, --input` | Specify program path | string |
| `-h, --help`  | Get help             | -      |

## Grammar

Is not hardcoded, described in `include/PrologGrammarChecker.hpp`

```C++
std::vector<prolog::grammar::Rule> rules =
            {
                {decl, equals{tok(prologTokens::NAME), body, str(".")}},

                {body, equals{str(":-"), disj}},
                {body, equals{}},

                {disj, equals{conj, oper(";"), disj}},
                {disj, equals{conj}},

                {conj, equals{name, str(","), conj}},
                {conj, equals{str("("), disj, str(")"), oper(","), conj}},
                {conj, equals{str("("), disj, str(")")}},
                {conj, equals{name}},

                {name, equals{tok(prologTokens::NAME)}},
            };
```

## Example

```bash
$ cat prolog.pl
a :- x, y.
b :- (x, y; a, b, c)
c :- x.
b :- (x, y a, b, c).
$ ./prolog-checker -i prolog.pl
c :- x.
^------
GrammarException: expected operator at 3:1
b :- (x, y a, b, c).
-----------^--------
GrammarException: unexpected token sequence after token at 4:12
```
