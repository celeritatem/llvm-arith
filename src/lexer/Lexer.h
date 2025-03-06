#ifndef LEXER_H
#define LEXER_H

#include "llvm/ADT/StringRef.h"

enum TokenTag : unsigned short {
  eof,
  unknown,
  identifier,
  number,
  comma,
  colon,
  plus,
  minus,
  star,
  slash,
  l_paren,
  r_paren,
  given
};

class Lexer;

class Token {
public:
  friend class Lexer;

  TokenTag tag;
  llvm::StringRef text;

  bool is(TokenTag t) const { return tag == t; };
  bool isOneOf(TokenTag t1, TokenTag t2) const {
    return is(t1) || is(t2);
  }
};

class Lexer {
  public:
  const char *start;
  const char *current;

  Lexer(const llvm::StringRef &buf) {
    start = buf.begin();
    current = start;
  }

  void next(Token &tok);

  void build_token(Token &tok, const char *end, TokenTag tag);
};

#endif
