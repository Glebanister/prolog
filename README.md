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

| Option         | Meaning                                                                      | Value  |
|----------------|------------------------------------------------------------------------------|--------|
| `-i, --input`  | Specify input file path                                                      | string |
| `-o, --output` | Specify output file path (output is being written to `<input-filename>.out`) | string |
| `-h, --help`   | Get help                                                                     | -      |
| `-t, --test`   | Run tests                                                                    | -      |

If program output is empty, then your program (or tests if there is `-t` flag) is correct.

## Grammar

Is not hardcoded, described in `src/PrologGrammarChecker.cpp`:

```C++
std::vector<prolog::grammar::Rule> rules =
    {
        {atom, equals{id, atom_seq}},

        {atom_seq, equals{atom}},
        {atom_seq, equals{}},
        {atom_seq, equals{bOpen, atom_brackets, bClose, atom_seq}},

        {atom_brackets, equals{bOpen, atom_brackets, bClose}},
        {atom_brackets, equals{atom}},

        {decl, equals{atom, corkscrew, body, period}},
        {decl, equals{atom, period}},

        {body, equals{disj}},

        {disj, equals{conj, opOr, disj}},
        {disj, equals{conj}},

        {conj, equals{atom, opAnd, conj}},
        {conj, equals{bOpen, disj, bClose, opAnd, conj}},
        {conj, equals{bOpen, disj, bClose}},
        {conj, equals{atom}},
    };
```

## Example

```bash
$ cat prolog.pl
a :- x, y.
b :- (x, y; a, b, c)
c :- x.
b :- (x, y a, b, c).
$ ./prolog-checker -t
$ ./prolog-checker -i prolog.pl
c :- x.
^------
GrammarException: expected operator at 3:1
b :- (x, y a, b, c).
-----------^--------
GrammarException: unexpected token sequence after token at 4:12
```
