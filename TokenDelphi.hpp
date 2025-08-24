#ifndef LEXER_TOKEN
#define LEXER_TOKEN

enum class TTokenID : long long {
    // Заглавные буквы A-Z
    A = 'A', B = 'B', C = 'C', D = 'D', E = 'E', F = 'F', G = 'G', H = 'H', I = 'I', J = 'J',
    K = 'K', L = 'L', M = 'M', N = 'N', O = 'O', P = 'P', Q = 'Q', R = 'R', S = 'S', T = 'T',
    U = 'U', V = 'V', W = 'W', X = 'X', Y = 'Y', Z = 'Z',

    // Строчные буквы a-z
    a = 'a', b = 'b', c = 'c', d = 'd', e = 'e', f = 'f', g = 'g', h = 'h', i = 'i', j = 'j',
    k = 'k', l = 'l', m = 'm', n = 'n', o = 'o', p = 'p', q = 'q', r = 'r', s = 's', t = 't',
    u = 'u', v = 'v', w = 'w', x = 'x', y = 'y', z = 'z',

    // Цифры 0-9
    Zero = '0', One = '1', Two = '2', Three = '3', Four = '4',
    Five = '5', Six = '6', Seven = '7', Eight = '8', Nine = '9',

    // Знаки препинания и символы
    Space = ' ',
    Exclamation = '!',   // !
    Quotation = '"',   // "
    Hash = '#',   // #
    Dollar = '$',   // $
    Percent = '%',   // %
    Ampersand = '&',   // &
    Apostrophe = '\'',  // '
    LeftParen = '(',   // (
    RightParen = ')',   // )
    LeftBrace = '{',   // {
    RightBrace = '}',   // }
    LeftBracket = '[',   // [
    RightBracket = ']',   // ]
    Asterisk = '*',   // *
    Plus = '+',   // +
    Comma = ',',   // ,
    Minus = '-',   // -
    Dot = '.',   // .
    Slash = '/',   // /
    Colon = ':',   // :
    Semicolon = ';',   // ;
    Less = '<',   // <
    Equals = '=',   // =
    Greater = '>',   // >
    Question = '?',   // ?
    At = '@',   // @
    Backslash = '\\',  // /
    Caret = '^',   // ^
    Underscore = '_',   // _
    Backtick = '`',   // `

    Pipe = '|',   // |

    Tilde = '~',   // ~

    // Управляющие символы (0–31, 127)
    Null = '\0',  // 0
    StartOfHeading = 1,     // SOH
    StartOfText = 2,     // STX
    EndOfText = 3,     // ETX
    EndOfTransmission = 4,  // EOT
    Enquiry = 5,     // ENQ
    Acknowledge = 6,     // ACK
    Bell = 7,     // '\a' (BEL)
    Backspace = 8,     // '\b' (BS)
    Tab = 9,     // '\t' (HT)
    LineFeed = 10,    // '\n' (LF)
    VerticalTab = 11,    // '\v' (VT)
    FormFeed = 12,    // '\f' (FF)
    CarriageReturn = 13,    // '\r' (CR)
    ShiftOut = 14,    // SO
    ShiftIn = 15,    // SI
    DataLinkEscape = 16,    // DLE
    DeviceControl1 = 17,    // DC1 (XON)
    DeviceControl2 = 18,    // DC2
    DeviceControl3 = 19,    // DC3 (XOFF)
    DeviceControl4 = 20,    // DC4
    NegativeAcknowledge = 21, // NAK
    SynchronousIdle = 22,   // SYN
    EndOfTransmissionBlock = 23, // ETB
    Cancel = 24,    // CAN
    EndOfMedium = 25,     // EM
    Substitute = 26,     // SUB
    Escape = 27,     // '\e' (ESC)
    FileSeparator = 28,     // FS
    GroupSeparator = 29,    // GS
    RecordSeparator = 30,   // RS
    UnitSeparator = 31,     // US
    Delete = 127,    // DEL
    
    // ===== Специальные токены =====
    FirstSpecialToken = 256,
    neof,          // Конец файла
    Unknown,      // Неизвестный символ
    Identifier,   // Идентификатор (varName, Class1)

    // ===== Литералы =====
    IntegerLiteral,  // 123, $FF
    FloatLiteral,    // 3.14, 2.5e10
    CharLiteral,     // 'A', #65
    StringLiteral,   // 'text', "text"
    HexLiteral,
    BinaryLiteral,
    BooleanLiteral,
    True,            // true
    False,           // false
    Nil,             // nil

    // ===== Операторы (расширенный) =====
    // Арифметические
    Div,         // div
    Mod,         // mod
    Square,         // mod

    // Логические/битовые
    And,         // and
    Or,          // or
    Not,         // not
    Xor,         // xor
    Shl,         // shl
    Shr,         // shr

    // Сравнения
    NotEqual,    // <>
    LessEqual,   // <=
    GreaterEqual,// >=

    // Присваивание
    Assign,      // :=

    // ===== Управляющие конструкции =====
    Begin,
    End,
    If,
    Then,
    Else,
    Case,
    For,
    To,
    Downto,
    While,
    Repeat,
    Until,
    Do,
    With,
    Try,
    Finally,
    Except,
    Raise,

    // ===== Объявление типов =====
    Type,
    Var,
    Const,
    Array,
    Record,
    Set,
    File,
    Pointer,
    Class,
    Interface,
    Object,

    // ===== Процедуры/функции =====
    Procedure,
    Function,
    Constructor,
    Destructor,
    Operator,
    Inline,
    Overload,
    Override,
    Virtual,
    Abstract,

    // ===== Модификаторы =====
    Private,
    Protected,
    Public,
    Published,
    Strict,
    Read,
    Write,
    Default,
    Stored,

    // ===== Модули =====
    Unit,
    Uses,
    Implementation,
    Initialization,
    Finalization,
    Library,
    Program,
    Package,

    // ===== Прочее =====
    Of,
    Is,
    As,
    In,
    Out,
    Inherited,
    Goto,
    Label,
    Exports,
    External,
    Message,
    Property,
    Packed,
    ThreadVar,
    Absolute,
    Assembler,
    Cdecl,
    SafeCall,
    StdCall,

    DispInterface,
    Reintroduce,
    Sealed,
    Static,
    Final,
    Platform,
    Deprecated,
    Experimental,
    Helper,
    Reference,
    Unsafe,

    // ===== Комментарии =====
    LineComment,    // //
    BlockComment,   // { } или (* *)

    // ===== Директивы компилятора =====
    CompilerDirective,  // {$IFDEF}
    DefineDirective,
    IfDefDirective,
    IfNDefDirective,
    EndIfDirective,
    IncludeDirective,
    ModeDirective,
    UndefDirective,
    IfDirective,
    ElseDirective

};

// Вспомогательная функция для преобразования enum в char
constexpr char constexprToChar(TTokenID s) {
    return static_cast<char>(s);
}

#define GENERATE_NAME(name) \
case TTokenID::name: return #name; \

std::string NameTTokenID(TTokenID kind) {

    switch (kind)
    {
    GENERATE_NAME(A);        
    GENERATE_NAME(B);        
    GENERATE_NAME(C);        
    GENERATE_NAME(D);        
    GENERATE_NAME(E);        
    GENERATE_NAME(F);        
    GENERATE_NAME(G);        
    GENERATE_NAME(H);        
    GENERATE_NAME(I);        
    GENERATE_NAME(J);        
    GENERATE_NAME(K);        
    GENERATE_NAME(L);        
    GENERATE_NAME(M);        
    GENERATE_NAME(N);        
    GENERATE_NAME(O);        
    GENERATE_NAME(P);        
    GENERATE_NAME(Q);        
    GENERATE_NAME(R);        
    GENERATE_NAME(S);        
    GENERATE_NAME(T);        
    GENERATE_NAME(U);        
    GENERATE_NAME(V);        
    GENERATE_NAME(W);        
    GENERATE_NAME(X);        
    GENERATE_NAME(Y);        
    GENERATE_NAME(Z);

    GENERATE_NAME(a);        
    GENERATE_NAME(b);        
    GENERATE_NAME(c);        
    GENERATE_NAME(d);        
    GENERATE_NAME(e);        
    GENERATE_NAME(f);        
    GENERATE_NAME(g);        
    GENERATE_NAME(h);        
    GENERATE_NAME(i);
    GENERATE_NAME(j);
    GENERATE_NAME(k);       
    GENERATE_NAME(l);
    GENERATE_NAME(m);
    GENERATE_NAME(n);
    GENERATE_NAME(o);
    GENERATE_NAME(p);
    GENERATE_NAME(q);
    GENERATE_NAME(r);
    GENERATE_NAME(s);
    GENERATE_NAME(t);
    GENERATE_NAME(u);
    GENERATE_NAME(v);
    GENERATE_NAME(w);
    GENERATE_NAME(x);
    GENERATE_NAME(y);
    GENERATE_NAME(z);
        
    GENERATE_NAME(Zero);
    GENERATE_NAME(One);
    GENERATE_NAME(Two);
    GENERATE_NAME(Three);
    GENERATE_NAME(Four);
    GENERATE_NAME(Five);
    GENERATE_NAME(Six);
    GENERATE_NAME(Seven);
    GENERATE_NAME(Eight);
    GENERATE_NAME(Nine);
        
    GENERATE_NAME(Space);
    GENERATE_NAME(Exclamation);
    GENERATE_NAME(Quotation);
    GENERATE_NAME(Hash);
    GENERATE_NAME(Dollar);
    GENERATE_NAME(Percent);
    GENERATE_NAME(Ampersand);
    GENERATE_NAME(Apostrophe);
    GENERATE_NAME(LeftParen);
    GENERATE_NAME(RightParen);
    GENERATE_NAME(LeftBrace);
    GENERATE_NAME(RightBrace);
    GENERATE_NAME(LeftBracket);
    GENERATE_NAME(RightBracket);
    GENERATE_NAME(Asterisk);
    GENERATE_NAME(Plus);
    GENERATE_NAME(Comma);
    GENERATE_NAME(Minus);
    GENERATE_NAME(Dot);
    GENERATE_NAME(Slash);
    GENERATE_NAME(Colon);
    GENERATE_NAME(Semicolon);
    GENERATE_NAME(Less);
    GENERATE_NAME(Equals);
    GENERATE_NAME(Greater);
    GENERATE_NAME(Question);
    GENERATE_NAME(At);
    GENERATE_NAME(Backslash);
    GENERATE_NAME(Caret);
    GENERATE_NAME(Underscore);
    GENERATE_NAME(Backtick);
    GENERATE_NAME(Pipe);
    GENERATE_NAME(Tilde);
    GENERATE_NAME(Null);
    GENERATE_NAME(StartOfHeading);
    GENERATE_NAME(StartOfText);
    GENERATE_NAME(EndOfText);
    GENERATE_NAME(EndOfTransmission);
    GENERATE_NAME(Enquiry);
    GENERATE_NAME(Acknowledge);
    GENERATE_NAME(Bell);
    GENERATE_NAME(Backspace);
    GENERATE_NAME(Tab);
    GENERATE_NAME(LineFeed);
    GENERATE_NAME(VerticalTab);
    GENERATE_NAME(FormFeed);
    GENERATE_NAME(CarriageReturn);

    GENERATE_NAME(ShiftOut);
    GENERATE_NAME(ShiftIn);
    GENERATE_NAME(DataLinkEscape);
    GENERATE_NAME(DeviceControl1);
    GENERATE_NAME(DeviceControl2);
    GENERATE_NAME(DeviceControl3);
    GENERATE_NAME(DeviceControl4);
    GENERATE_NAME(NegativeAcknowledge);
    GENERATE_NAME(SynchronousIdle);
    GENERATE_NAME(EndOfTransmissionBlock);
    GENERATE_NAME(Cancel);
    GENERATE_NAME(EndOfMedium);
    GENERATE_NAME(Substitute);
    GENERATE_NAME(Escape);
    GENERATE_NAME(FileSeparator);
    GENERATE_NAME(GroupSeparator);
    GENERATE_NAME(RecordSeparator);
    GENERATE_NAME(UnitSeparator);
    GENERATE_NAME(Delete);

    GENERATE_NAME(FirstSpecialToken);
    GENERATE_NAME(neof);
    
        
    GENERATE_NAME(Identifier);
    GENERATE_NAME(IntegerLiteral);
    GENERATE_NAME(FloatLiteral);
    GENERATE_NAME(CharLiteral);
    GENERATE_NAME(StringLiteral);
    GENERATE_NAME(HexLiteral);
    GENERATE_NAME(BinaryLiteral);
    GENERATE_NAME(BooleanLiteral);

    GENERATE_NAME(True);
    GENERATE_NAME(False);
    GENERATE_NAME(Nil);
        
    GENERATE_NAME(Div);        
    GENERATE_NAME(Mod);        
    GENERATE_NAME(Square);
    GENERATE_NAME(And);        
    GENERATE_NAME(Or);        
    GENERATE_NAME(Not);        
    GENERATE_NAME(Xor);        
    GENERATE_NAME(Shl);        
    GENERATE_NAME(Shr);        
    GENERATE_NAME(NotEqual);        
    GENERATE_NAME(LessEqual);        
    GENERATE_NAME(GreaterEqual);        
    GENERATE_NAME(Assign);     
    GENERATE_NAME(Begin);
    GENERATE_NAME(End);        
    GENERATE_NAME(If);
    GENERATE_NAME(Then);        
    GENERATE_NAME(Else);
    GENERATE_NAME(Case);
        
    GENERATE_NAME(For);
        
    GENERATE_NAME(To);
        
    GENERATE_NAME(Downto);
        
    GENERATE_NAME(While);
        
    GENERATE_NAME(Repeat);
        
    GENERATE_NAME(Until);
        
    GENERATE_NAME(Do);
        
    GENERATE_NAME(With);
        
    GENERATE_NAME(Try);
        
    GENERATE_NAME(Finally);
        
    GENERATE_NAME(Except);
        
    GENERATE_NAME(Raise);
        
    GENERATE_NAME(Type);
        
    GENERATE_NAME(Var);
        
    GENERATE_NAME(Const);
        
    GENERATE_NAME(Array);
        
    GENERATE_NAME(Record);
        
    GENERATE_NAME(Set);
        
    GENERATE_NAME(File);
        
    GENERATE_NAME(Pointer);
        
    GENERATE_NAME(Class);
        
    GENERATE_NAME(Interface);
        
    GENERATE_NAME(Object);
        
    GENERATE_NAME(Procedure);
        
    GENERATE_NAME(Function);
        
    GENERATE_NAME(Constructor);
        
    GENERATE_NAME(Destructor);
        
    GENERATE_NAME(Operator);
        
    GENERATE_NAME(Inline);
        
    GENERATE_NAME(Overload);
        
    GENERATE_NAME(Override);
        
    GENERATE_NAME(Virtual);
        
    GENERATE_NAME(Abstract);
        
    GENERATE_NAME(Private);
        
    GENERATE_NAME(Protected);
        
    GENERATE_NAME(Public);
        
    GENERATE_NAME(Published);
        
    GENERATE_NAME(Strict);
        
    GENERATE_NAME(Read);
        
    GENERATE_NAME(Write);
        
    GENERATE_NAME(Default);
        
    GENERATE_NAME(Stored);
        
    GENERATE_NAME(Unit);
        
    GENERATE_NAME(Uses);
        
    GENERATE_NAME(Implementation);
        
    GENERATE_NAME(Initialization);
        
    GENERATE_NAME(Finalization);
        
    GENERATE_NAME(Library);
        
    GENERATE_NAME(Program);
        
    GENERATE_NAME(Package);
        
    GENERATE_NAME(Of);
        
    GENERATE_NAME(Is);
        
    GENERATE_NAME(As);
        
    GENERATE_NAME(In);
        
    GENERATE_NAME(Out);
        
    GENERATE_NAME(Inherited);
        
    GENERATE_NAME(Goto);
        
    GENERATE_NAME(Label);
        
    GENERATE_NAME(Exports);
        
    GENERATE_NAME(External);
        
    GENERATE_NAME(Message);
        
    GENERATE_NAME(Property);
        
    GENERATE_NAME(Packed);
        
    GENERATE_NAME(ThreadVar);
        
    GENERATE_NAME(Absolute);
        
    GENERATE_NAME(Assembler);
        
    GENERATE_NAME(Cdecl);
        
    GENERATE_NAME(SafeCall);
        
    GENERATE_NAME(StdCall);
        
    GENERATE_NAME(DispInterface);
        
    GENERATE_NAME(Reintroduce);
        
    GENERATE_NAME(Sealed);
        
    GENERATE_NAME(Static);
        
    GENERATE_NAME(Final);
        
    GENERATE_NAME(Platform);
        
    GENERATE_NAME(Deprecated);
        
    GENERATE_NAME(Experimental);
        
    GENERATE_NAME(Helper);
        
    GENERATE_NAME(Reference);
        
    GENERATE_NAME(Unsafe);
        
    GENERATE_NAME(LineComment);
        
    GENERATE_NAME(BlockComment);
        
    GENERATE_NAME(CompilerDirective);
        
    GENERATE_NAME(DefineDirective);
        
    GENERATE_NAME(IfDefDirective);
        
    GENERATE_NAME(IfNDefDirective);
        
    GENERATE_NAME(EndIfDirective);        
    GENERATE_NAME(IncludeDirective);      
    GENERATE_NAME(ModeDirective);  
    GENERATE_NAME(UndefDirective);    
    GENERATE_NAME(IfDirective);  
    GENERATE_NAME(ElseDirective);
    default:
        return "Unknow";
    }
}


enum class LexErrorType {
    UnclosedString,    // Не закрыта кавычка '
    UnclosedDirective, // Не закрыта директива
    UnclosedComment,   // Не закрыт комментарий { или (*
    InvalidNumber,     // Например, 1.2.3 или 0xGG
    InvalidChar,       // Недопустимый символ
};

// Функция, принимающая enum и строковый литерал + callback
std::string GetErrorString(LexErrorType type) {
    switch (type) {
    case LexErrorType::UnclosedString:    return "UnclosedString";
    case LexErrorType::UnclosedDirective:       return "UnclosedDirective";
    case LexErrorType::UnclosedComment: return "UnclosedComment";
    case LexErrorType::InvalidNumber:       return "InvalidNumber";
    case LexErrorType::InvalidChar:       return "InvalidChar";
    }

}

using Callback = void(*)(std::string);
Callback LexError;


#endif // LEXER_TOKEN
