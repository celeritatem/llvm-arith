#include "CodeGen.h"
#include "parser/AST.h"
#include "llvm/ADT/StringMap.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/GlobalVariable.h"

using namespace llvm;

namespace {
class ToIRVisitor : public ASTVisitor {
  Module *mod;
  IRBuilder<> builder;
  Type *void_ty;
  Type *int32_ty;
  PointerType *ptr_ty;
  Constant *int32_0;
  Value *value;
  StringMap<Value *> name_map;

public:
  ToIRVisitor(Module *m) : mod(m), builder(m->getContext()) {
    void_ty = Type::getVoidTy(m->getContext());
    int32_ty = Type::getInt32Ty(m->getContext());
    ptr_ty = PointerType::getUnqual(m->getContext());
    int32_0 = ConstantInt::get(int32_ty, 0, true);
  }

  void run(AST *st) {
    // define the main function prototype and function in the IR
    FunctionType *main_fn_ty =
        FunctionType::get(int32_ty, {int32_ty, ptr_ty}, false);
    Function *main_fn =
        Function::Create(main_fn_ty, GlobalValue::ExternalLinkage, "main", mod);
    // Create entry basic block
    BasicBlock *entry_bb =
        BasicBlock::Create(mod->getContext(), "entry", main_fn);
    builder.SetInsertPoint(entry_bb);
    // Traverse the syntax tree
    st->accept(*this);
    // Print computed value via call to `arith_write`
    FunctionType *arith_write_fn_ty =
        FunctionType::get(void_ty, {int32_ty}, false);
    Function *arith_write_fn = Function::Create(
        arith_write_fn_ty, GlobalValue::ExternalLinkage, "arith_write", mod);
    builder.CreateCall(arith_write_fn_ty, arith_write_fn, {value});
    // ret 0 from main()
    builder.CreateRet(int32_0);
  }

  // Visit methods:
  // ==============
  virtual void visit(GivenDecl &node) override {
    // Read variables via call to `arith_read`
    FunctionType *read_fn_ty = FunctionType::get(int32_ty, {ptr_ty}, false);
    Function *read_fn = Function::Create(
        read_fn_ty, GlobalValue::ExternalLinkage, "arith_read", mod);
    // loop through var names
    for (auto curr = node.begin(), end = node.end(); curr != end; curr++) {
      // create var name string
      auto var = *curr;
      Constant *str_text = ConstantDataArray::getString(mod->getContext(), var);
      GlobalVariable *str = new GlobalVariable(
          *mod, str_text->getType(), true, GlobalVariable::PrivateLinkage,
          str_text, Twine(var).concat(".str"));
      // read value via call
      CallInst *call = builder.CreateCall(read_fn_ty, read_fn, {str});
      // store given value
      name_map[var] = call;
    }
    node.expr->accept(*this);
  }

  virtual void visit(Factor &node) override {
    if (node.tag == Factor::identifier) {
        value = name_map[node.value];
    } else {
        int intval;
        node.value.getAsInteger(10, intval);
        value = ConstantInt::get(int32_ty, intval, true);
    }
  }

  virtual void visit(BinaryOp &node) override {
    node.lhs->accept(*this);
    Value *lhs = value;
    node.rhs->accept(*this);
    Value *rhs = value;
    switch(node.op) {
        case BinaryOp::plus:
            value = builder.CreateNSWAdd(lhs, rhs);
            break;
        case BinaryOp::minus:
            value = builder.CreateNSWSub(lhs, rhs);
            break;
        case BinaryOp::mul:
            value = builder.CreateNSWMul(lhs, rhs);
            break;
        case BinaryOp::div:
            value = builder.CreateSDiv(lhs, rhs);
            break;
    }
  }

};
} // namespace


void CodeGen::compile(AST *st) {
    LLVMContext ctx;
    Module *mod = new Module("arith.expr", ctx);
    ToIRVisitor visitor(mod);
    visitor.run(st);
    mod->print(outs(), nullptr);
}