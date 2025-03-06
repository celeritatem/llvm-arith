# LLVM Arithmetic Expression Compiler

A simple arithmetic expression language using LLVM.

## References:
- Learn LLVM 17 by Nacke & Kwan
- LLVM Tutorial - LLVM Docs


## Quickstart

### Build the compiler:

```bash
export LLVM_DIR=<path_to_llvm_source>
cmake -S . -B build
CC=clang CXX=clang++ cmake --build build
```

### Compile arithmetic expressions:

```bash
# compile an arithmetic expression
build/arithc "given a: a+1" | llc -filetype=obj -relocation-model=pic -o=program1.o
# link with runtime
clang -o program1 program1.o src/runtime/arith_runtime.c
# execture
./program1
```


## Grammar

```EBNF
arith : ("given" identifier ("," identifier)* ":")? expr ;
expr : term (( "+" | "-" ) term)* ;
term : factor (( "*" | "/" ) factor)* ;
factor : identifier | number | "(" expr ")" ;
identifier: ([a-zA-Z])+ ;
number : ([0-9])+ ;
```


