# define exec and src files
add_executable(arith
		Compiler.cpp lexer/Lexer.cpp parser/AST.cpp parser/Parser.cpp sema/Sema.cpp codegen/CodeGen.cpp)
# link against libraries
target_link_libraries(arith PRIVATE ${llvm_libs})
