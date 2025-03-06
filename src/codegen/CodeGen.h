#ifndef CODEGEN_H
#define CODEGEN_H

#include "parser/AST.h"

class CodeGen {
public:
  void compile(AST *st);
};

#endif