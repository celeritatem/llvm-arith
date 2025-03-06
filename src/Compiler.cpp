#include "codegen/CodeGen.h"
#include "parser/Parser.h"
#include "sema/Sema.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/InitLLVM.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

static cl::opt<std::string> input(cl::Positional, cl::desc("<input expr>"),
                                  cl::init(""));

int main(int argc, const char **argv) {
  InitLLVM X(argc, argv);
  cl::ParseCommandLineOptions(argc, argv, "arithmetic expressions compiler\n");

  // Parse
  Lexer lexer(input);
  Parser parser(lexer);
  AST *st = parser.parse();
  if (!st || parser.has_error) {
    errs() << "syntax errors\n";
    return 1;
  }

  // Semantic analysis
  Sema sa;
  if (sa.semantic(st)) {
    errs() << "semantic errors\n";
    return 1;
  }

  // Generate LLVM IR
  CodeGen codegen;
  codegen.compile(st);

  return 0;
}