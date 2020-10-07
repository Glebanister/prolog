# Prolog grammar checker

Formal languages HW №5.

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
$ ./prolog-checker -i example.pl && cat example.pl.out
f () :- a.
---^------
GrammarException: expected opening bracket ('(') at 1:4

a :- b.
├─ function declaration: 'a :- b .'
│  ├─ atom: 'a'
│  │  ├─ token of category 'identifier': 'a'
│  │  ├─ atom sequence: ''
│  ├─ corckscrew (':-'): ':-'
│  ├─ function body: 'b'
│  │  ├─ disjunction: 'b'
│  │  │  ├─ conjunction: 'b'
│  │  │  │  ├─ atom: 'b'
│  │  │  │  │  ├─ token of category 'identifier': 'b'
│  │  │  │  │  ├─ atom sequence: ''
│  ├─ period ('.'): '.'
```
