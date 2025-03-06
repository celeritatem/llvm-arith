#include "AST.h"


void Factor::accept(ASTVisitor &visitor) { visitor.visit(*this); }

void BinaryOp::accept(ASTVisitor &visitor) { visitor.visit(*this); }

void GivenDecl::accept(ASTVisitor &visitor) { visitor.visit(*this); }
