// ParserDelphi.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <iostream>
#include <fstream>
#include <filesystem>

#include "ParserDelphi.hpp"

std::string ReadFile(std::string filepath) {

    std::ifstream file(filepath, std::ios::binary | std::ios::ate);
    if (!file)
        return "";
    auto size = file.tellg();
    std::string content(size, '\0');
    file.seekg(0);
    file.read(&content[0], size);
    return content;
};

void std_cout(std::string out) {
    std::cout << out;
}

std::string code = R"(
EUpdateError = class(EDatabaseError)
  private
    FErrorCode: Integer;
    FPreviousError: Integer;
    FPosition: Integer;
    FContext: string;
    FOriginalException: Exception;
  public
    function MatchBinarOperatorWithTuple(SContext: TSContext; CArray: TIDExpression; SecondArg: TIDExpression): TIDDeclaration;
    constructor Create(NativeError, Context: string;
      ErrCode, PrevError: Integer; E: Exception);
    destructor Destroy; override;
    property Context: string read FContext;
    property ErrorCode: Integer read FErrorCode;
    property PreviousError: Integer read FPreviousError;
    property OriginalException: Exception read FOriginalException;
    property Position: Integer read FPosition write FPosition;
  end;
)";

std::string CodeFile = ReadFile("");

int main()
{

    LexError = std_cout;
    ParseError = std_cout;

    LexerEngine lexEngine(code);
    auto lexbuffer = lexEngine.GetBufferToken();

    ParserEngine parser(lexbuffer);
    parser.ParserRun();

    if (false)
    {
        for (auto& Tok : lexbuffer) {
            std::cout << NameTTokenID(Tok.type) << " |" << Tok.value << "|\n";
        }
    }

}