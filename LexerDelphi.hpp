#include <string>
#include <vector>
#include <algorithm>
#include <string_view>
#include <utility>
#include <array>

#include <unordered_map>

#include "TokenDelphi.hpp"
#include "TokenKeywordMap.hpp"
#include "TokenDirectiveMap.hpp"

#define DEF_GENERATION_TO_CHAR(X) \
push_back_token_storage(); \
LexToken LexToken{ X, std::string(1, constexprToChar(X)), CurrentLine, CurrentColumn }; \
BufferToken.push_back(LexToken); \

#define DEF_GENERATION_BASE(X) \
push_back_token_storage(); \
LexToken LexToken{ TTokenID::##X, std::string(1, constexprToChar(TTokenID::##X)), CurrentLine, CurrentColumn }; \
BufferToken.push_back(LexToken); \

struct LexToken {
    TTokenID type;
    std::string value;
    size_t line = 0;
    size_t column = 0;

    bool operator == (const LexToken& other) const {
        return type == other.type;
    };
};

class LexerEngine {
private:
    using LexEnginePtr = void (LexerEngine::*)();
private:
    int CurrentLine = 0;
    int CurrentColumn = 0;
    int PosBuffer = 0;
    std::string storage_value = "";
private:
    void ProcessSpace() { DEF_GENERATION_BASE(Space);};
    void ProcessLineFeed() { DEF_GENERATION_BASE(LineFeed);};

    void ProcessQuotation() { DEF_GENERATION_BASE(Quotation); };
    void ProcessHash() { DEF_GENERATION_BASE(Hash); };
    void ProcessDollar() { DEF_GENERATION_BASE(Dollar); };
    void ProcessPercent() { DEF_GENERATION_BASE(Percent); };
    void ProcessAmpersand() { DEF_GENERATION_BASE(Ampersand); };
    void ProcessRightParen() { DEF_GENERATION_BASE(RightParen); };
    void ProcessRightBrace() { DEF_GENERATION_BASE(RightBrace); };
    void ProcessLeftBracket() { DEF_GENERATION_BASE(LeftBracket); };
    void ProcessRightBracket() { DEF_GENERATION_BASE(RightBracket); };
    void ProcessPlus() { DEF_GENERATION_BASE(Plus); };
    void ProcessComma() { DEF_GENERATION_BASE(Comma); };
    void ProcessMinus() { DEF_GENERATION_BASE(Minus); };
    void ProcessDot() { DEF_GENERATION_BASE(Dot); };
    void ProcessColon() { DEF_GENERATION_BASE(Colon); };
    void ProcessSemicolon() { DEF_GENERATION_BASE(Semicolon); };
    void ProcessLess() { DEF_GENERATION_BASE(Less); };
    void ProcessEquals() { DEF_GENERATION_BASE(Equals); };
    void ProcessGreater() { DEF_GENERATION_BASE(Greater); };
    void ProcessAt() { DEF_GENERATION_BASE(At); };
    void ProcessBackslash() { DEF_GENERATION_BASE(Backslash); };
    void ProcessCaret() { DEF_GENERATION_BASE(Caret); };
    void ProcessUnderscore() { DEF_GENERATION_BASE(Underscore); };
    void ProcessBacktick() { DEF_GENERATION_BASE(Backtick); };
    void ProcessPipe() { DEF_GENERATION_BASE(Pipe); };
    void ProcessTilde() { DEF_GENERATION_BASE(Tilde); };

    void ProcessCarriageReturn();
    void ProcessAsterisk();
    void ProcessApostrophe();
    void ProcessLeftParen();
    void ProcessLeftBrace();
    void ProcessSlash();


    std::unordered_map<char, LexEnginePtr> map{ {
    {' ', &LexerEngine::ProcessSpace},
    {'\'', &LexerEngine::ProcessQuotation},
    {'#', &LexerEngine::ProcessHash},
    {'$', &LexerEngine::ProcessDollar},
    {'%', &LexerEngine::ProcessPercent},
    {'&', &LexerEngine::ProcessAmpersand},
    {')', &LexerEngine::ProcessRightParen},
    {'}', &LexerEngine::ProcessRightBrace},
    {'[', &LexerEngine::ProcessLeftBracket},
    {']', &LexerEngine::ProcessRightBracket},
    {'+', &LexerEngine::ProcessPlus},
    {',', &LexerEngine::ProcessComma},
    {'-', &LexerEngine::ProcessMinus},
    {'.', &LexerEngine::ProcessDot},
    {':', &LexerEngine::ProcessColon},
    {';', &LexerEngine::ProcessSemicolon},
    {'<', &LexerEngine::ProcessLess},
    {'=', &LexerEngine::ProcessEquals},
    {'>', &LexerEngine::ProcessGreater},
    {'@', &LexerEngine::ProcessAt},
    {'\\', &LexerEngine::ProcessBackslash},
    {'^', &LexerEngine::ProcessCaret},
    {'_', &LexerEngine::ProcessUnderscore},
    {'`', &LexerEngine::ProcessBacktick},
    {'|', &LexerEngine::ProcessPipe},

    {'\n', &LexerEngine::ProcessLineFeed},
    {'\r', &LexerEngine::ProcessCarriageReturn},
    {'*', &LexerEngine::ProcessAsterisk},
    {'\'', &LexerEngine::ProcessApostrophe},
    {'(', &LexerEngine::ProcessLeftParen},
    {'{', &LexerEngine::ProcessLeftBrace},
    {'/', &LexerEngine::ProcessSlash},

}};

    // Обновляем проверку идентификаторов:
    bool is_unicode_identifier_start(char32_t c) {
        if (((c >= constexprToChar(TTokenID::A) && c <= constexprToChar(TTokenID::Z)) ||
            (c >= constexprToChar(TTokenID::a) && c <= constexprToChar(TTokenID::z)) ||
            c == constexprToChar(TTokenID::Underscore)))
            return true;
        return (c >= 0xC0); // Все символы выше ASCII
    }
    bool is_digit(char32_t c) {
        return (c >= constexprToChar(TTokenID::Zero) && c <= constexprToChar(TTokenID::Nine));
    }
    bool neof() {
        return PosBuffer < SourceCode.size();
    }

    void parse_number();
    void push_back_token_storage();
    
    const char GetChar();

    void LexerRun();

    std::string SourceCode = "";
    std::vector<LexToken> BufferToken;
public:
	LexerEngine(const std::string source) { 
        SourceCode = source; 
        LexerRun();
    };

    std::vector<LexToken> GetBufferToken () {
        return BufferToken;
    }
};

void LexerEngine::LexerRun() {

    while (neof()) {
        if (is_unicode_identifier_start(GetChar()) || is_digit(GetChar())) {
            
            if (is_digit(GetChar()))
                parse_number();
            
            size_t start = PosBuffer;
            while (neof() && (is_unicode_identifier_start(GetChar()) || is_digit(GetChar()))) {
                PosBuffer++;
            }

            std::string identifier = SourceCode.substr(start, PosBuffer - start);       
            PosBuffer--;

            // Проверяем, является ли собранная строка ключевым словом
            auto it = TokenKeywordMap.find(PascalHash(identifier));
            bool IsKeyword = it != TokenKeywordMap.end();

            LexToken LexToken{ 
                IsKeyword ? it->second : TTokenID::Identifier,
                identifier, 
                CurrentLine, 
                CurrentColumn };
            BufferToken.push_back(LexToken);

            CurrentLine += identifier.size();
        }
        else
        {

            if (auto it = map.find(GetChar()); it != map.end()) {
                (this->*it->second)();
            }
            else
            {
                storage_value += GetChar();
            }
        }
        CurrentLine++;
        PosBuffer++;
    }

}

const char LexerEngine::GetChar() {
    return SourceCode[PosBuffer];
}

void LexerEngine::push_back_token_storage() {
    if (!storage_value.empty()) {
        LexToken LexToken{ TTokenID::Identifier, storage_value, CurrentLine, CurrentColumn };
        BufferToken.push_back(LexToken);
        storage_value = "";
    }
}

void LexerEngine::parse_number() {
    push_back_token_storage();

    size_t start = PosBuffer;
    bool is_hex = false;
    bool is_float = false;

    // Проверяем на шестнадцатеричное число ($FF)
    if (GetChar() == '0' && PosBuffer + 1 < SourceCode.size() &&
        (SourceCode[PosBuffer + 1] == 'x' || SourceCode[PosBuffer + 1] == 'X')) {
        is_hex = true;
        PosBuffer += 2;
    }

    while (neof()) {
        if (is_hex) {
            if (!isxdigit(GetChar())) break;  // Только 0-9, A-F
        }
        else {
            if (GetChar() == '.') {
                if (is_float) break;  // Уже была точка → ошибка
                is_float = true;
            }
            else if (GetChar() == 'e' || GetChar() == 'E') {
                if (PosBuffer + 1 < SourceCode.size() &&
                    (SourceCode[PosBuffer + 1] == '+' || SourceCode[PosBuffer + 1] == '-')) {
                    PosBuffer++;  // Пропускаем знак экспоненты
                }
                is_float = true;
            }
            else if (GetChar() < '0' || GetChar() > '9') {
                break;
            }
        }
        PosBuffer++;
    }

    std::string num_str = SourceCode.substr(start, PosBuffer - start);
    TTokenID kind =
        is_float ? TTokenID::FloatLiteral :
        is_hex ? TTokenID::HexLiteral :
        TTokenID::IntegerLiteral;

    LexToken LexToken{ kind, num_str, CurrentLine, CurrentColumn };
    BufferToken.push_back(LexToken);
    CurrentLine += num_str.size();
    PosBuffer--;
}

void LexerEngine::ProcessCarriageReturn() {
    push_back_token_storage();
    // Если после `\r` идёт `\n` (Windows: `\r\n`), пропускаем `\n`
    if (PosBuffer + 1 < SourceCode.size() && SourceCode[PosBuffer + 1] == '\n') {
        PosBuffer++;  // Пропускаем `\n`, чтобы не дублировать LineFeed
    }
}

void LexerEngine::ProcessAsterisk() {
    push_back_token_storage();
    PosBuffer++;
    bool IsSquare = GetChar() == '*';

    LexToken LexToken{
        IsSquare ? TTokenID::Square : TTokenID::Asterisk,
        IsSquare ? "**" : std::string(1, constexprToChar(TTokenID::Asterisk)),
        CurrentLine,
        CurrentColumn };
    BufferToken.push_back(LexToken);
}

void LexerEngine::ProcessApostrophe() {
    push_back_token_storage();
    std::string str_literal;
    PosBuffer++;  // Пропускаем открывающую кавычку

    while (neof()) {
        if (GetChar() == '\'') {
            // Обработка экранированной кавычки
            if (PosBuffer + 1 < SourceCode.size() && SourceCode[PosBuffer + 1] == '\'') {
                str_literal += '\'';
                PosBuffer += 2;
            }
            else {
                PosBuffer++;
                break;
            }
        }
        else if (GetChar() == '#') {
            // Обработка управляющих последовательностей (#13, #$A и т.д.)
            PosBuffer++;
            if (neof() && GetChar() == '$') {
                // Шестнадцатеричная последовательность (#$A)
                PosBuffer++;
                std::string hex_seq;
                while (neof() && isxdigit(GetChar())) {
                    hex_seq += GetChar();
                    PosBuffer++;
                }
                if (!hex_seq.empty()) {
                    str_literal += static_cast<char>(std::stoi(hex_seq, nullptr, 16));
                }
            }
            else {
                // Десятичная последовательность (#13)
                std::string dec_seq;
                while (neof() && isdigit(GetChar())) {
                    dec_seq += GetChar();
                    PosBuffer++;
                }
                if (!dec_seq.empty()) {
                    str_literal += static_cast<char>(std::stoi(dec_seq));
                }
            }
        }
        else {
            str_literal += GetChar();
            PosBuffer++;
        }
    }
    LexToken token{ TTokenID::StringLiteral, str_literal, CurrentLine, CurrentColumn };
    BufferToken.push_back(token);
}

void LexerEngine::ProcessLeftParen() {
    push_back_token_storage();
    if (PosBuffer + 1 < SourceCode.size() && SourceCode[PosBuffer + 1] == '*') {

        PosBuffer += 2;  // Пропускаем "(*"
        while (neof() && !(GetChar() == '*' && PosBuffer + 1 < SourceCode.size() && SourceCode[PosBuffer + 1] == ')')) {
            if (GetChar() == '\n') {
                CurrentColumn++;
            }
            PosBuffer++;
        }

        if (!neof()) {
            if (LexError)
                LexError(GetErrorString(LexErrorType::UnclosedComment));
        }
        else {
            PosBuffer += 2;  // Пропускаем "*)"
        }
    }
    else
    {
        // Если не комментарий, то это символ '('
        DEF_GENERATION_TO_CHAR(TTokenID::LeftParen);

    }
}

void LexerEngine::ProcessLeftBrace() {
    push_back_token_storage();

    if (PosBuffer + 1 < SourceCode.size() && SourceCode[PosBuffer + 1] == '$')
    {
        PosBuffer += 2; // Skip "{$"
        size_t directive_start = PosBuffer;

        while (PosBuffer < SourceCode.size() && is_unicode_identifier_start(GetChar())) {
            PosBuffer++;
        }

        std::string directive_name = SourceCode.substr(directive_start, PosBuffer - directive_start);
        std::transform(directive_name.begin(), directive_name.end(), directive_name.begin(), ::toupper);

        // Skip whitespace after directive name
        while (PosBuffer < SourceCode.size() && isspace(GetChar())) {
            PosBuffer++;
        }

        // Get directive content (until '}')
        std::string directive_content;
        while (PosBuffer < SourceCode.size() && GetChar() != '}') {
            directive_content += GetChar();
            PosBuffer++;
        }

        // Remove trailing whitespace from content
        while (!directive_content.empty() && isspace(directive_content.back())) {
            directive_content.pop_back();
        }

        // Create appropriate LexToken based on directive type
        auto it = TokenDirectiveMap.find(directive_name);
        bool IsSearchMap = it != TokenDirectiveMap.end();

        LexToken LexToken{
            IsSearchMap ? it->second : TTokenID::CompilerDirective,
            directive_content,
            CurrentLine,
            CurrentColumn
        };

        BufferToken.push_back(LexToken);

        if (PosBuffer > SourceCode.size()) {
            if (LexError) {
                LexError(GetErrorString(LexErrorType::UnclosedDirective));
            }
        }
    }
    else
    {
        size_t start_line = CurrentLine;
        while (neof() && GetChar() != '}') {
            if (GetChar() == '\n')
                CurrentColumn++;
            PosBuffer++;
        }
        if (!neof())
            if (LexError)
                LexError(GetErrorString(LexErrorType::UnclosedComment));

    }
}

void LexerEngine::ProcessSlash() {
    push_back_token_storage();
    if (PosBuffer + 1 < SourceCode.size() && SourceCode[PosBuffer + 1] == '/') {

        while (neof() && GetChar() != '\n') {
            PosBuffer++;
        }
    }
    else
    {
        DEF_GENERATION_TO_CHAR(TTokenID::Slash);
    }
}
