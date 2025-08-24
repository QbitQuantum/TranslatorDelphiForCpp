#ifndef LEXER_TOKEN_KEYWORDMAP
#define LEXER_TOKEN_KEYWORDMAP

#include <unordered_map>
#include "TokenDelphi.hpp"

constexpr char const_tolower(char c) {
    return (c >= 'A' && c <= 'Z') ? c + ('a' - 'A') : c;
}

constexpr uint32_t ConstexprPascalHash(const char* str, uint32_t seed = 0) {
    uint32_t hash = seed;
    for (size_t i = 0; str[i] != '\0'; ++i) {
        uint32_t k = static_cast<uint8_t>(const_tolower(str[i]));  // Игнорируем регистр
        k *= 0xcc9e2d51;
        k = (k << 15) | (k >> 17);
        k *= 0x1b873593;
        hash ^= k;
        hash = (hash << 13) | (hash >> 19);
        hash = hash * 5 + 0xe6546b64;
    }
    hash ^= std::char_traits<char>::length(str);
    hash ^= hash >> 16;
    hash *= 0x85ebca6b;
    hash ^= hash >> 13;
    hash *= 0xc2b2ae35;
    hash ^= hash >> 16;
    return hash;
}

uint32_t PascalHash(const char* str, uint32_t seed = 0) {
    uint32_t hash = seed;
    for (size_t i = 0; str[i] != '\0'; ++i) {
        uint32_t k = static_cast<uint8_t>(const_tolower(str[i]));  // Игнорируем регистр
        k *= 0xcc9e2d51;
        k = (k << 15) | (k >> 17);
        k *= 0x1b873593;
        hash ^= k;
        hash = (hash << 13) | (hash >> 19);
        hash = hash * 5 + 0xe6546b64;
    }
    hash ^= std::char_traits<char>::length(str);
    hash ^= hash >> 16;
    hash *= 0x85ebca6b;
    hash ^= hash >> 13;
    hash *= 0xc2b2ae35;
    hash ^= hash >> 16;
    return hash;
}

uint32_t PascalHash(const std::string& s, uint32_t seed = 0) {
    uint32_t hash = seed;
    for (char c : s) {
        uint32_t k = static_cast<uint8_t>(tolower(c));
        k *= 0xcc9e2d51;
        k = (k << 15) | (k >> 17);
        k *= 0x1b873593;
        hash ^= k;
        hash = (hash << 13) | (hash >> 19);
        hash = hash * 5 + 0xe6546b64;
    }
    hash ^= s.size();
    hash ^= hash >> 16;
    hash *= 0x85ebca6b;
    hash ^= hash >> 13;
    hash *= 0xc2b2ae35;
    hash ^= hash >> 16;
    return hash;
}

static const std::unordered_map<uint32_t, TTokenID> TokenKeywordMap = {

    // ===== Литералы =====
    {ConstexprPascalHash("true"),        {TTokenID::True}},
    {ConstexprPascalHash("false"),        {TTokenID::False}},
    {ConstexprPascalHash("nil"),        {TTokenID::Nil}},

    // ===== Операторы (расширенный) =====
    // Арифметические
    {ConstexprPascalHash("div"),        {TTokenID::Div}},
    {ConstexprPascalHash("mod"),        {TTokenID::Mod}},

    // Логические/битовые
    {ConstexprPascalHash("and"),        {TTokenID::And}},
    {ConstexprPascalHash("or"),         {TTokenID::Or}},
    {ConstexprPascalHash("not"),        {TTokenID::Not}},
    {ConstexprPascalHash("xor"),        {TTokenID::Xor}},
    {ConstexprPascalHash("Shl"),        {TTokenID::Shl}},
    {ConstexprPascalHash("Shr"),        {TTokenID::Shr}},

    // Сравнения
    {ConstexprPascalHash("<>"),        {TTokenID::NotEqual}},
    {ConstexprPascalHash("<="),        {TTokenID::LessEqual}},
    {ConstexprPascalHash(">="),        {TTokenID::GreaterEqual}},

    // Присваивание
    {ConstexprPascalHash(":="),           {TTokenID::Assign}},

    // ===== Управляющие конструкции =====
    {ConstexprPascalHash("begin"),        {TTokenID::Begin}},
    {ConstexprPascalHash("end"),          {TTokenID::End}},
    {ConstexprPascalHash("if"),           {TTokenID::If}},
    {ConstexprPascalHash("then"),         {TTokenID::Then}},
    {ConstexprPascalHash("else"),         {TTokenID::Else}},
    {ConstexprPascalHash("case"),         {TTokenID::Case}},
    {ConstexprPascalHash("for"),          {TTokenID::For}},
    {ConstexprPascalHash("to"),          {TTokenID::To}},
    {ConstexprPascalHash("downto"),          {TTokenID::Downto}},
    {ConstexprPascalHash("while"),        {TTokenID::While}},
    {ConstexprPascalHash("repeat"),       {TTokenID::Repeat}},
    {ConstexprPascalHash("until"),        {TTokenID::Until}},
    {ConstexprPascalHash("do"),           {TTokenID::Do}},
    {ConstexprPascalHash("with"),         {TTokenID::With}},
    {ConstexprPascalHash("try"),          {TTokenID::Try}},
    {ConstexprPascalHash("finally"),      {TTokenID::Finally}},
    {ConstexprPascalHash("except"),       {TTokenID::Except}},
    {ConstexprPascalHash("raise"),        {TTokenID::Raise}},

    // ===== Объявление типов =====
    {ConstexprPascalHash("type"),        {TTokenID::Type}},
    {ConstexprPascalHash("var"),         {TTokenID::Var}},
    {ConstexprPascalHash("const"),       {TTokenID::Const}},
    {ConstexprPascalHash("array"),       {TTokenID::Array}},
    {ConstexprPascalHash("record"),      {TTokenID::Record}},
    {ConstexprPascalHash("set"),         {TTokenID::Set}},
    {ConstexprPascalHash("file"),        {TTokenID::File}},
    {ConstexprPascalHash("pointer"),     {TTokenID::Pointer}},
    {ConstexprPascalHash("class"),       {TTokenID::Class}},
    {ConstexprPascalHash("interface"),   {TTokenID::Interface}},
    {ConstexprPascalHash("object"),      {TTokenID::Object}},

    // ===== Процедуры/функции =====
    {ConstexprPascalHash("procedure"),    {TTokenID::Procedure}},
    {ConstexprPascalHash("function"),     {TTokenID::Function}},
    {ConstexprPascalHash("constructor"),  {TTokenID::Constructor}},
    {ConstexprPascalHash("destructor"),   {TTokenID::Destructor}},
    {ConstexprPascalHash("operator"),     {TTokenID::Operator}},
    {ConstexprPascalHash("inline"),       {TTokenID::Inline}},
    {ConstexprPascalHash("overload"),     {TTokenID::Overload}},
    {ConstexprPascalHash("override"),     {TTokenID::Override}},
    {ConstexprPascalHash("virtual"),      {TTokenID::Virtual}},
    {ConstexprPascalHash("abstract"),     {TTokenID::Abstract}},

    // ===== Модификаторы =====
    {ConstexprPascalHash("private"),      {TTokenID::Private}},
    {ConstexprPascalHash("protected"),    {TTokenID::Protected}},
    {ConstexprPascalHash("public"),       {TTokenID::Public}},
    {ConstexprPascalHash("published"),    {TTokenID::Published}},
    {ConstexprPascalHash("strict"),       {TTokenID::Strict}},
    {ConstexprPascalHash("read"),         {TTokenID::Read}},
    {ConstexprPascalHash("write"),        {TTokenID::Write}},
    {ConstexprPascalHash("default"),      {TTokenID::Default}},
    {ConstexprPascalHash("stored"),       {TTokenID::Stored}},

    // ===== Модули =====
    {ConstexprPascalHash("unit"),        {TTokenID::Unit}},
    {ConstexprPascalHash("uses"),        {TTokenID::Uses}},
    {ConstexprPascalHash("implementation"), {TTokenID::Implementation}},
    {ConstexprPascalHash("initialization"), {TTokenID::Initialization}},
    {ConstexprPascalHash("finalization"), {TTokenID::Finalization}},
    {ConstexprPascalHash("library"),     {TTokenID::Library}},
    {ConstexprPascalHash("program"),     {TTokenID::Program}},
    {ConstexprPascalHash("package"),     {TTokenID::Package}},

    // ===== Прочее =====
    {ConstexprPascalHash("of"),          {TTokenID::Of}},
    {ConstexprPascalHash("is"),          {TTokenID::Is}},
    {ConstexprPascalHash("as"),          {TTokenID::As}},
    {ConstexprPascalHash("in"),          {TTokenID::In}},
    {ConstexprPascalHash("out"),         {TTokenID::Out}},
    {ConstexprPascalHash("inherited"),   {TTokenID::Inherited}},
    {ConstexprPascalHash("goto"),        {TTokenID::Goto}},
    {ConstexprPascalHash("packed"),       TTokenID::Packed},  // Добавлено
    {ConstexprPascalHash("label"),       {TTokenID::Label}},
    {ConstexprPascalHash("exports"),     {TTokenID::Exports}},
    {ConstexprPascalHash("external"),     {TTokenID::External} },
    {ConstexprPascalHash("message"),      {TTokenID::Message} },
    {ConstexprPascalHash("property"),     {TTokenID::Property} },
    {ConstexprPascalHash("threadvar"),    {TTokenID::ThreadVar} },
    {ConstexprPascalHash("absolute"),     {TTokenID::Absolute} },
    {ConstexprPascalHash("assembler"),   {TTokenID::Assembler} },
    {ConstexprPascalHash("cdecl"),      {TTokenID::Cdecl} },
    {ConstexprPascalHash("safecall"),   {TTokenID::SafeCall} },
    {ConstexprPascalHash("dispinterface"), TTokenID::DispInterface },
    {ConstexprPascalHash("reintroduce"),    TTokenID::Reintroduce },
    {ConstexprPascalHash("sealed"),         TTokenID::Sealed },
    {ConstexprPascalHash("static"),         TTokenID::Static },
    {ConstexprPascalHash("final"),          TTokenID::Final },
    {ConstexprPascalHash("platform"),       TTokenID::Platform },
    {ConstexprPascalHash("deprecated"),     TTokenID::Deprecated },
    {ConstexprPascalHash("experimental"),   TTokenID::Experimental },
    {ConstexprPascalHash("helper"),         TTokenID::Helper },
    {ConstexprPascalHash("reference"),      TTokenID::Reference },
    {ConstexprPascalHash("unsafe"),         TTokenID::Unsafe }

     
};

static_assert(ConstexprPascalHash("end") != ConstexprPascalHash("var"));

#endif //LEXER_TOKEN_KEYWORDMAP