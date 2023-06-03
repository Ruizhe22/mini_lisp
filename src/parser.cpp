//
// Created by Ray Chou on 2023/5/7.
//

#include "parser.h"

Parser::Parser(std::deque<TokenPtr> tokens):tokenPtrDeque(std::move(tokens)) {}

ValuePtr Parser::parse() {
        auto& firstTokenPtr = tokenPtrDeque.front();

        if (firstTokenPtr->getType() == TokenType::NUMERIC_LITERAL) {
            auto value = static_cast<NumericLiteralToken&>(*firstTokenPtr).getValue();
            tokenPtrDeque.pop_front();
            return std::make_shared<NumericValue>(value);
        }
        else if(firstTokenPtr->getType() == TokenType::BOOLEAN_LITERAL){
            auto value = static_cast<BooleanLiteralToken&>(*firstTokenPtr).getValue();
            tokenPtrDeque.pop_front();
            return std::make_shared<BooleanValue>(value);
        }
        else if(firstTokenPtr->getType() == TokenType::STRING_LITERAL){
            auto value = static_cast<StringLiteralToken&>(*firstTokenPtr).getValue();
            tokenPtrDeque.pop_front();
            return std::make_shared<StringValue>(value);
        }
        else if(firstTokenPtr->getType() == TokenType::LEFT_PAREN){
            tokenPtrDeque.pop_front();
            return parseTails();
        }
        else if (firstTokenPtr->getType() == TokenType::QUOTE) {
            tokenPtrDeque.pop_front();
            return std::make_shared<PairValue>(
                std::make_shared<SymbolValue>("quote"),
                    std::make_shared<PairValue>(
                        parse(),
                        std::make_shared<NilValue>()
                            )
            );
        }
        else if (firstTokenPtr->getType() == TokenType::QUASIQUOTE) {
            tokenPtrDeque.pop_front();
            return std::make_shared<PairValue>(
                std::make_shared<SymbolValue>("quasiquote"),
                std::make_shared<PairValue>(
                    parse(),
                    std::make_shared<NilValue>()
                        )
            );
        }else if (firstTokenPtr->getType() == TokenType::UNQUOTE) {
            tokenPtrDeque.pop_front();
            return std::make_shared<PairValue>(
                std::make_shared<SymbolValue>("unquote"),
                std::make_shared<PairValue>(
                    parse(),
                    std::make_shared<NilValue>()
                        )
            );
        }else if(firstTokenPtr->getType() == TokenType::IDENTIFIER){
            auto value = static_cast<IdentifierToken&>(*firstTokenPtr).getName();
            tokenPtrDeque.pop_front();
            return std::make_shared<SymbolValue>(value);
        }
        else{
            throw SyntaxError("First Token Error.");
        }

}

//只要遇到左括号，就是一个列表（列表去掉car之后还是一个列表，虽然此时没有左括号）
// 列表用parseTails返回一个对子，且对子的第一个ValuePtr由parse得到。

ValuePtr Parser::parseTails() {

        if(tokenPtrDeque.empty()) throw SyntaxError("No Right Paren");

        auto firstToken = *tokenPtrDeque.front();
        if ( firstToken.getType() == TokenType::RIGHT_PAREN) {
            tokenPtrDeque.pop_front();
            return std::make_shared<NilValue>();
        }
        auto car = parse();

        if(tokenPtrDeque.empty()) throw SyntaxError("No Right Paren");

        firstToken = *tokenPtrDeque.front();
        if ( firstToken.getType() == TokenType::DOT) {
            tokenPtrDeque.pop_front();

            if(tokenPtrDeque.empty()) throw SyntaxError("No Right Paren");

            auto cdr = parse();

            if(tokenPtrDeque.empty()) throw SyntaxError("No Right Paren");

            firstToken = *tokenPtrDeque.front();
            if(firstToken.getType() == TokenType::RIGHT_PAREN){
                return std::make_shared<PairValue>(car, cdr);
            }
            else{
                throw SyntaxError("No Right Paren");
            }
        }
        else {
            auto cdr = parseTails();
            return std::make_shared<PairValue>(car, cdr);
        }
}

