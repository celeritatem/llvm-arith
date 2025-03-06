#ifndef AST_H
#define AST_H

#include "llvm/ADT/SmallVector.h"
#include "llvm/ADT/StringRef.h"

class ASTVisitor;

class AST {
public:
  virtual ~AST() {}
  virtual void accept(ASTVisitor &V) = 0;
};

class Expr : public AST {
public:
  Expr() {}
};

class Factor : public Expr {
public:
  enum Tag { identifier, number };

  Tag tag;
  llvm::StringRef value;

  Factor(Tag t, llvm::StringRef val) : tag(t), value(val) {}

  virtual void accept(ASTVisitor &visitor) override;

};

class BinaryOp : public Expr {
public:
  enum Operator { plus, minus, mul, div };

  Expr *lhs;
  Expr *rhs;
  Operator op;

  BinaryOp(Operator o, Expr *l, Expr *r) : op(o), lhs(l), rhs(r) {}

  virtual void accept(ASTVisitor &visitor) override;
};

class GivenDecl : public AST {
public:
  llvm::SmallVector<llvm::StringRef, 8> vars;
  Expr *expr;

  GivenDecl(llvm::SmallVector<llvm::StringRef, 8> v, Expr *e)
      : vars(v), expr(e) {}

  llvm::SmallVector<llvm::StringRef, 8>::const_iterator begin() {
    return vars.begin();
  }

  llvm::SmallVector<llvm::StringRef, 8>::const_iterator end() {
    return vars.end();
  }

  virtual void accept(ASTVisitor &visitor) override;
};

class ASTVisitor {
public:
  virtual void visit(AST &) {};
  virtual void visit(Factor &) = 0;
  virtual void visit(BinaryOp &) = 0;
  virtual void visit(GivenDecl &) = 0;
};

#endif
