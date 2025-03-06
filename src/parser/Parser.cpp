#include "Parser.h"
#include "AST.h"

AST *Parser::parse() {
  AST *program = parse_arith();
  expect(TokenTag::eof);
  return program;
}

AST *Parser::parse_arith() {
  Expr *expr;
  llvm::SmallVector<llvm::StringRef, 8> vars;

  if (token.is(TokenTag::given)) {
    advance();
  }

  if (expect(TokenTag::identifier)) {
    // TODO
    llvm::errs() << "missing variable declarations\n";
    // error
    advance_to_end();
  }
  vars.push_back(token.text);
  advance();

  while (token.is(TokenTag::comma)) {
    advance();
    if (expect(TokenTag::identifier)) {
      // TODO
      llvm::errs() << "expected variable name after ','\n";
      // error
      advance_to_end();
    }
    vars.push_back(token.text);
    advance();
  }

  //
  if (consume(TokenTag::colon)) {
    // TODO
    llvm::errs() << "missing ':' after variable declarations\n";
    // error
      advance_to_end();
  }

  expr = parse_expr();

  if (vars.empty()) {
    return expr;
  } else {
    return new GivenDecl(vars, expr);
  }
};

AST *Parser::advance_to_end() {
  while (!token.is(TokenTag::eof)) {
    advance();
  }
  return nullptr;
}

Expr *Parser::parse_expr() {
  Expr *lhs = parse_term();
  while (token.isOneOf(TokenTag::plus, TokenTag::minus)) {
    BinaryOp::Operator op;
    if (token.is(TokenTag::plus)) {
      op = BinaryOp::Operator::plus;
    } else {
      op = BinaryOp::Operator::minus;
    }
    advance();
    Expr *rhs = parse_term();
    lhs = new BinaryOp(op, lhs, rhs);
  }
  return lhs;
};

Expr *Parser::parse_term() {
  Expr *lhs = parse_factor();
  while (token.isOneOf(TokenTag::star, TokenTag::slash)) {
    BinaryOp::Operator op;
    if (token.is(TokenTag::star)) {
      op = BinaryOp::Operator::mul;
    } else {
      op = BinaryOp::Operator::div;
    }
    advance();
    Expr *rhs = parse_factor();
    lhs = new BinaryOp(op, lhs, rhs);
  }
  return lhs;
};

Expr *Parser::parse_factor() {
  Expr *expr = nullptr;
  switch (token.tag) {
  case TokenTag::number:
    expr = new Factor(Factor::Tag::number, token.text);
    advance();
    break;
  case TokenTag::identifier:
    expr = new Factor(Factor::Tag::identifier, token.text);
    advance();
    break;
  case TokenTag::l_paren:
    advance();
    expr = parse_expr();
    if (!consume(TokenTag::r_paren)) {
      break;
    } else {
      // TODO
      llvm::errs() << "missing ')'?\n";
      advance_to_end();
    }
  default:
    if (!expr) {
      // error
      error();
    }
  }
  return expr;
}
