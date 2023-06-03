//
// Created by Ray Chou on 2023/5/7.
//

#ifndef MINI_LISP_PARSER_H
#define MINI_LISP_PARSER_H

#include "./value.h"
#include "./token.h"
#include "./error.h"

#include <deque>

class Parser {
private:
    std::deque<TokenPtr> tokenPtrDeque;
public:
    Parser(std::deque<TokenPtr> tokens);
    ValuePtr parse();
    ValuePtr parseTails();
};

#endif  // MINI_LISP_PARSER_H
