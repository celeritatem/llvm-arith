#include "Sema.h"
#include "parser/AST.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/ADT/StringSet.h"
#include "llvm/Support/raw_ostream.h"

namespace {
class DeclCheck : public ASTVisitor {
public:
  llvm::StringSet<> scope;
  bool has_error = false;

  enum ErrorType { undeclared, redeclared };

  void error(ErrorType et, llvm::StringRef var) {
    if (et == ErrorType::undeclared) {
      llvm::errs() << "cannot use undeclared variable: " << var << "\n";
    } else {
      llvm::errs() << "cannot declare existing variable: " << var << "\n";
    }
    has_error = true;
  }

  DeclCheck() {}

  // Visit methods:
  // ===============
  virtual void visit(Factor &node) override {
    if (node.tag == Factor::identifier) {
      if (scope.find(node.value) == scope.end()) {
        error(undeclared, node.value);
      }
    }
  }

  virtual void visit(BinaryOp &node) override {
    if (node.lhs) {
      node.lhs->accept(*this);
    } else {
      has_error = true;
    }
    if (node.lhs) {
      node.rhs->accept(*this);
    } else {
      has_error = true;
    }
  }

  virtual void visit(GivenDecl &node) override {
    for (auto curr = node.begin(), last = node.end(); curr != last; curr++) {
      if (!scope.insert(*curr).second) {
        error(redeclared, *curr);
      }
    }
    if (node.expr) {
      node.expr->accept(*this);
    } else {
      has_error = true;
    }
  }
};

} // namespace

/// Entry point to start the semantic analysis tree walk.
bool Sema::semantic(AST *st) {
  if (!st) {
    return false;
  }
  DeclCheck decl_check;
  st->accept(decl_check);
  return decl_check.has_error;
};