#include "Lexer.h"
#include "llvm/ADT/StringRef.h"

namespace chars {
    inline bool is_whitespace(char c) {
        return c == ' ' || c == '\t' || c == '\f' || c == '\v' || c == '\r' || c == '\n';
    }

    inline bool is_digit(char c) {
        return c >= '0' && c <= '9';
    }

    inline bool is_letter(char c) {
        return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
    }
}

void Lexer::next(Token &tok) {
    // skip whitespace
    while (*current && chars::is_whitespace(*current)) {
        current++;
    }

    // chars left?
    if (!*current) {
        tok.tag = TokenTag::eof;
        return;
    }


    // if alphanumeric, read the full token:
    const char *end = current + 1;
    if (chars::is_letter(*current)) {
        // Keywords or variable identifiers
        while (chars::is_letter(*end)) {
            end++;
        }
        llvm::StringRef identifier_name(current, end - current);
        if (identifier_name == "given") {
            tok.tag = TokenTag::given;
        } else {
            tok.tag = TokenTag::identifier;
        }
        tok.text = llvm::StringRef(current, end - current);
        current = end;
 
        return;
    } else if (chars::is_digit(*current)) {

        // Numbers
        while (chars::is_digit(*end)) {
            end++;
        }
        tok.tag = TokenTag::number;
        tok.text = llvm::StringRef(current, end - current);
        current = end;

        return;
    }

    // Read single char tokens:
    switch(*current) {
        case '+':
            tok.tag = TokenTag::plus;
            break;
        case '-':
            tok.tag = TokenTag::minus;
            break;
        case '*':
            tok.tag = TokenTag::star;
            break;
        case '/':
            tok.tag = TokenTag::slash;
            break;
        case '(':
            tok.tag = TokenTag::l_paren;
            break;
        case ')':
            tok.tag = TokenTag::r_paren;
            break;
        case ':':
            tok.tag = TokenTag::colon;
            break;
        case ',':
            tok.tag = TokenTag::comma;
            break;
        default:
            tok.tag = TokenTag::unknown;
            break;
    }
    tok.text = llvm::StringRef(current, 1);
    current++;
    return;
}