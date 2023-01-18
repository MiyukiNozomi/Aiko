#pragma once

#include <string>
#include <ctype.h>
#include <fstream>
#include <sstream>
#include <iostream>

void PrintError(std::string message);
void ParseParameter(std::string arg);

////////////////////////////
// Lexing Stage
////////////////////////////

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

////////////////////////////
// Utilities
////////////////////////////

// Console coloring
enum Color {
#ifdef _WIN32
    Black = 0,
    Blue = 1,
    Green = 2,
    Cyan = 3,
    Red = 4,
    Purple = 5,
    Gold = 6,
    Gray = 7,
    DarkGray = 8,
    LightBlue = 9,
    LightGreen = 10,
    LightCyan = 11,
    LightRed = 12,
    Magenta = 13,
    Yellow = 14,
    White = 15,
#else
    Black          = 40,
    Blue           = 44,
    Green          = 42,
    Cyan           = 46,
    Red            = 41,
    Purple         = 45,
    Gold           = 43,
    Gray           = 107, 
    DarkGray       = 100,
    LightBlue    = 104,
    LightGreen   = 102,
    LightCyan    = 106,
    LightRed     = 101,
    Yellow       = 103,
    Magenta      = 105,
    White        = 47,
#endif
};

#ifdef _WIN32

#include <Windows.h>

namespace Terminal {
    static bool onTerminal = true;
    static HANDLE hConsole;
    static CONSOLE_SCREEN_BUFFER_INFO sbi;

    void Init() {
        hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

        if (GetConsoleScreenBufferInfo(hConsole, &sbi) == 0)
            onTerminal = false;
    }

    void SetColor(int foreground) {
        if (!onTerminal)
            return;
        SetConsoleTextAttribute(hConsole, (short) (foreground));
    }
    
    void ResetColor() {
        if(!onTerminal) return;
            SetConsoleTextAttribute(hConsole, sbi.wAttributes);
    }
    
    void Logo(const char* version) {
#ifdef _WIN32
#define SetMagenta SetConsoleTextAttribute(hConsole, (short)(Color::Magenta * 16 + Color::Black));
#else
#define SetMagenta printf("\1b[95m");
#endif
        SetMagenta
        printf("     ");

        SetColor(Color::LightCyan);
        printf("          Aiko's Forge ");
        SetColor(Color::Gold);
        printf("%s\n", version);

        SetMagenta
#ifdef _WIN32
        printf("  Ai ");
#else
        printf("  \u2764 ");
#endif
#ifdef DEBUG
        SetColor(Color::LightRed);
        printf("           -- debug mode --");
#endif
        printf("\n\n");

        ResetColor();
#undef SetMagenta
    }
}
#else
namespace Terminal {
    void Init() {}

    void SetColor(int foreground) {
        printf("\1b[%dm", foreground);
    }
    
    void ResetColor() {
        printf("\x1b[0m");
    }
}
#endif

// checks if a string ends with something, like an extension
bool EndsWith(std::string const &str, std::string const &suffix) {
    if (str.length() < suffix.length()) {
        return false;
    }
    return str.compare(str.length() - suffix.length(), suffix.length(), suffix) == 0;
}