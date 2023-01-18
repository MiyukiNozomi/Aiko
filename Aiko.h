#pragma once

#include <string>
#include <ctype.h>
#include <fstream>
#include <sstream>
#include <iostream>

void ParseParameter(std::string arg);

// lexer
enum TokenType {
    Invalid = 0,
    EndOfFile = -1,

    // keywords
    KeyLet    = -2,
    KeyExtern = -3,

    OpEquals = -4,
    OpPlus   = -5,
    OpMinus  = -6,
    OpDivide = -8,
    OpMultiply   = -7,
    
    Semicolon = -8,

    // literals
    Identifier = -8,
    Integer = -9,
    Float   = -10
};

struct Token {
    public:
        std::string value;
        int line;
        TokenType type;

        Token(TokenType type, std::string& value, int line) {
            this->type = type;
            this->value = value;
            this->line = line;
        }
};

#define Symbol(c, type) if (current == c) { return Token(type, std::string() + NextChar(), this->lines);} 

class Lexer {
    public:
        Lexer(std::string& source) {
            this->source   = source;
            this->lines    = 1;
            this->position = 0;
            this->NextChar();
        }

        Token NextToken() {
            while (isspace(current) && current != '\0') {
                NextChar();
            }

            if (current == '\0') {
                return Token(TokenType::EndOfFile, std::string(), this->lines);
            }

            if (isdigit(current)) {
                TokenType t = TokenType::Integer;
                std::string txt = "";

                while ((isdigit(current) || current == '.') && current != '\0') {
                    if (current == '.')
                        t = TokenType::Float;
                    txt += NextChar();
                }

                return Token(t, txt, this->lines);
            }

            if (isalpha(current) || current == '_') {
                std::string id = "";

                while ((isalpha(current) || current == '_' || isdigit(current)) && current != '\0') {
                    id += NextChar();
                }

                return Token(TokenType::Identifier, id, this->lines);
            }

            Symbol('=', TokenType::OpEquals)

            Symbol(';', TokenType::Semicolon)

            Symbol('+', TokenType::OpPlus)
            Symbol('-', TokenType::OpMinus)
            Symbol('/', TokenType::OpDivide)
            Symbol('*', TokenType::OpMultiply)

            NextChar();
            return Token(TokenType::Invalid, std::string(), this->lines);
        }

    private:
        std::string source;
        size_t position;
        char current;
        int lines;

        char NextChar() {
            char last = current;
            if (position >= source.length()) {
                current = '\0';
            } else {
                current = source[position++];
                if (current == '\n')
                    ++lines;
            }
            return last;
        }
};

bool EndsWith(std::string const &str, std::string const &suffix) {
    if (str.length() < suffix.length()) {
        return false;
    }
    return str.compare(str.length() - suffix.length(), suffix.length(), suffix) == 0;
}