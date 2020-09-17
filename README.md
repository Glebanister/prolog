# Lexer

Formal languages HW №2.

## Installation

```bash
git clone https://github.com/Glebanister/fp_lexer --recursive
cd fp_lexer
mkdir build && cd build
cmake ..
make
```

## Usage

```bash
./fp_lexer <input file name>
```

## Example

```bash
$ cat example
sig auto.

type state, initial, final State -> o.
$ ./fp_lexer example 
$ cat example.sig 
SIG 'sig' 0 0
NAME 'auto' 0 4
DOT '.' 0 8
TYPE 'type' 2 11
NAME 'state' 2 16
COMMA ',' 2 21
NAME 'initial' 2 23
COMMA ',' 2 30
NAME 'final' 2 32
NAME 'State' 2 38
OPERATOR_BI '->' 2 44
NAME 'o' 2 47
DOT '.' 2 48
```
