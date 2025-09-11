#ifndef LEXER_TOKEN_DERECTIVEMAP
#define LEXER_TOKEN_DERECTIVEMAP

#include <string>
#include <unordered_map>
#include "TokenDelphi.hpp"

const std::unordered_map<std::string, TTokenID> TokenDirectiveMap = {
    {"IFDEF",    TTokenID::IfDefDirective},
    {"IFNDEF",   TTokenID::IfNDefDirective},
    {"ENDIF",    TTokenID::EndIfDirective},
    {"DEFINE",   TTokenID::DefineDirective},
    {"INCLUDE",  TTokenID::IncludeDirective},
    {"MODE",     TTokenID::ModeDirective},
    {"UNDEF",   TTokenID::UndefDirective},
    {"IF",  TTokenID::IfDirective},
    {"ELSE",     TTokenID::ElseDirective},
};

#endif // LEXER_TOKEN_DERECTIVEMAP