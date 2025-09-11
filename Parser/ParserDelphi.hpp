

#include "LexerDelphi.hpp"
#include "AstDelphi.hpp"
#include <vector>
#include <utility>

using CallbackParse = void(*)(std::string);
CallbackParse ParseError;

namespace kind {
	constexpr int Program = 0;
	constexpr int Unit = 1;
	constexpr int If = 2;
	constexpr int Else = 3;
	constexpr int Body = 4;
}

namespace Scope {
	constexpr int Private = 0;
	constexpr int Public = 1;
	constexpr int Protect = 3;
}

namespace direction {
	constexpr bool next = true;
	constexpr bool previous = false;
}

class ParserEngine {
	
	struct property_method
	{
		int TypeScope;
		std::string name_method;
		std::string type_method;
		std::string name_read;
		std::string name_write;
	};

	struct BlockDeclareArgument {
		std::string Qualifier;
		std::vector<std::string> Arguments;
		std::string TypeArgument;
		std::string DefaultValue;
	};

	struct procedure_class
	{
		int TypeScope;
		std::string procedure_name;
		std::vector<BlockDeclareArgument> body_function;
	};
	
	struct function_class
	{
		int TypeScope;
		std::string function_name;
		std::vector<BlockDeclareArgument> body_function;
		std::string return_type;
	};
	
	struct constructor_class
	{
		int TypeScope;
		std::string constructor_name;
		std::vector<BlockDeclareArgument> body_function;
	};

	struct destructor_class
	{
		int TypeScope;
		std::string destructor_name;
		bool IsOvveride;
	};

	int PosBuffer = 0;

	std::vector<LexToken> ParserBuffer;
	std::vector<std::pair<int, AST*>> AstBuffer;
	
	void Shift(bool _direction);
	
	void ClearJunkToken(bool _direction);
	
	bool matchCurrentToken(TTokenID kind);
	bool IsQualifierToken();

	bool JunkToken();
	bool neof() { return PosBuffer < ParserBuffer.size();}

	LexToken GetToken();

	std::vector<LexToken> GetLexToken(int amount, bool _direction);

	bool parseUnit();
	bool parseClass();
	bool parseScope(int& TypeScope);
	bool parseProperty(std::vector<property_method>& _property_method);
	bool parseFunctions(std::vector<function_class>& _functions);
	bool parseProcedure(std::vector<procedure_class>& _procedure);
	bool parseConstructor(constructor_class& constructor);
	bool parseDestructor(destructor_class& destructor);
	bool parseDeclareFunction(std::vector<BlockDeclareArgument>& Declare);
	bool parseDeclareVar(std::vector<std::pair<std::string, std::string>> Var);

public:
	ParserEngine(std::vector<LexToken> Buffer) { ParserBuffer = Buffer; };
	~ParserEngine() { for (auto& [left, right] : AstBuffer) delete right; };
	void ParserRun();
};

void ParserEngine::ParserRun() {
	while (neof())
	{
		TTokenID Type = ParserBuffer[PosBuffer].type;
		switch (Type)
		{
		case TTokenID::Unit: {
			if (!parseUnit())
				return;
			break;
		}
		case TTokenID::Class: {
			if (!parseClass())
				return;
			break;
		}
		default:

			break;
		};
		
		Shift(direction::next);
	}
}

bool ParserEngine::parseUnit() {
	// Пропускаем 'unit'
	Shift(direction::next);
	std::vector<std::string> UnitName;

	while (neof() && !matchCurrentToken(TTokenID::Semicolon)) {
		if (matchCurrentToken(TTokenID::Identifier) || matchCurrentToken(TTokenID::Dot))
		{
			if (matchCurrentToken(TTokenID::Identifier)) {
				UnitName.push_back(ParserBuffer[PosBuffer].value);
				PosBuffer++;
			}
			else if (matchCurrentToken(TTokenID::Dot)) {
				if (UnitName.empty() || (PosBuffer + 1 >= ParserBuffer.size()) || (ParserBuffer[PosBuffer + 1].type != TTokenID::Identifier)) {
					if (ParseError) ParseError("Invalid unit name syntax");
					return false;
				}
				UnitName.push_back(std::string(1, constexprToChar(TTokenID::Dot)));
				PosBuffer++;
			}
		}
		else
		{
			if (ParseError) ParseError("Not valid token");
			
			return false;
		}
	}
	if (UnitName.empty()) {
		if (ParseError) ParseError("Unit name cannot be empty");
		return false;
	}
	
	UnitNode* unitNode = new UnitNode(UnitName);
	AstBuffer.push_back({ kind::Unit, unitNode });

	return true;
};

bool ParserEngine::parseClass() {

	const std::vector<LexToken> match = {
		LexToken{TTokenID::Equals, "=", 1, 1},
		LexToken{TTokenID::Identifier, "", 1, 1} 
	};

	int oldPos = PosBuffer;
	std::vector<LexToken> buffer = GetLexToken(match.size(), direction::previous);
	PosBuffer = oldPos;

	if (buffer != match)
	{
		if (ParseError) ParseError("No correct MatchPattern class"); return false;
	}

	// Имя класса
	std::string class_name = "";
	// Наследники
	std::vector<std::string> legatee;
	// Декларация переменных
	std::vector<std::pair<std::string, std::string>> declaration_param;
	// Декларация методов (property)	
	std::vector<property_method> property_methods;
	// Декларация конструктора
	constructor_class Consructor;
	// Декларация десруктора
	destructor_class Destructor;
	// Декларация функций
	std::vector<function_class> functions;
	// Декларация процедур
	std::vector<procedure_class> procedures;

	class_name = buffer[1].value;
	
	Shift(direction::next);

	// Проверяем, наследуются ли другие классы
	if (matchCurrentToken(TTokenID::LeftParen))
	{
		while (neof() && !matchCurrentToken(TTokenID::RightParen)) {
			if (matchCurrentToken(TTokenID::Identifier))
				legatee.push_back(ParserBuffer[PosBuffer].value);
			Shift(direction::next);
		}
	}

	// Пропускаем RightParen
	Shift(direction::next);

	int TypeScope = Scope::Public;
	
	// Парсим тело класса
	while (neof() && !matchCurrentToken(TTokenID::End)) {
		LexToken token = GetToken();

		switch (token.type)
		{
			// Проверяем видимость методов
		case TTokenID::Public:
		case TTokenID::Protected:
		case TTokenID::Private:
			if (!parseScope(TypeScope))
				return false;
			Shift(direction::next);
			break;
		case TTokenID::Constructor:
			Consructor.TypeScope = TypeScope;
			if (!parseConstructor(Consructor))
				return false;
			Shift(direction::next);
			break;
		case TTokenID::Destructor:
			Destructor.TypeScope = TypeScope;
			if (!parseDestructor(Destructor))
				return false;
			Shift(direction::next);
			break;
		case TTokenID::Property:
			property_methods.push_back({});
			property_methods.back().TypeScope = TypeScope;
			if (!parseProperty(property_methods))
				return false;
			Shift(direction::next);
			break;
		case TTokenID::Function:
			functions.push_back({});
			functions.back().TypeScope = TypeScope;
			if (!parseFunctions(functions))
				return false;
			Shift(direction::next);
			break;
		case TTokenID::Procedure:
			procedures.push_back({});
			procedures.back().TypeScope = TypeScope;
			if (!parseProcedure(procedures))
				return false;
			Shift(direction::next);
			break;
		case TTokenID::Identifier:
		{
			std::string SafeValueToken = token.value;
			const std::vector<LexToken> matchDeclareVar = {
				LexToken{TTokenID::Colon, ":", 1, 1},
				LexToken{TTokenID::Identifier, "", 1, 1},
				LexToken{TTokenID::Semicolon, ";", 1, 1},
			};

			std::vector<LexToken> bufferDeclareVar = GetLexToken(matchDeclareVar.size(), direction::next);

			if (matchDeclareVar == bufferDeclareVar)
				declaration_param.push_back({ SafeValueToken, bufferDeclareVar[1].value });
			Shift(direction::next);
		}
			break;
		default:
			Shift(direction::next);
			break;
		}
	}
	return true;
};

bool ParserEngine::parseProperty(std::vector<property_method>& _property_method) {
	
	std::string property_name = "";
	std::string property_type = "";
	std::string property_read_name = "";
	std::string property_write_name = "";

	//Пропускаем текущий токен
	Shift(direction::next);

	if (!matchCurrentToken(TTokenID::Identifier))
	{
		if (ParseError) ParseError("No find name property"); return false;
	}

	property_name = GetToken().value;
	
	Shift(direction::next);

	if (!matchCurrentToken(TTokenID::Colon))
	{
		if (ParseError) ParseError("No find start declaration method property"); return false;
	}

	Shift(direction::next);

	if (!matchCurrentToken(TTokenID::Identifier))
	{
		if (ParseError) ParseError("No find type property method"); return false;
	}

	property_type = GetToken().value;


	while (neof() && !matchCurrentToken(TTokenID::Semicolon)) {
		if (matchCurrentToken(TTokenID::Read))
		{
			Shift(direction::next);
			if (!matchCurrentToken(TTokenID::Identifier))
			{
				if (ParseError) ParseError("No correct name read property"); return false;
			}
			property_read_name = GetToken().value;
		}

		if (matchCurrentToken(TTokenID::Write))
		{
			Shift(direction::next);
			if (!matchCurrentToken(TTokenID::Identifier))
			{
				if (ParseError) ParseError("No correct name write property"); return false;
			}
			property_write_name = GetToken().value;
		}
		Shift(direction::next);
	}

	auto& Save = _property_method.back();
	Save.name_method = property_name;
	Save.type_method = property_type;
	Save.name_read = property_read_name;
	Save.name_write = property_write_name;
	return true;
};

bool ParserEngine::parseFunctions(std::vector<function_class>& _functions) {

	// Пропускаем текущий токен
	Shift(direction::next);

	if (!matchCurrentToken(TTokenID::Identifier))
	{
		if (matchCurrentToken(TTokenID::LeftParen))
		{
			if (ParseError) ParseError("No find name constructor_class");
			return false;
		}
		if (ParseError) ParseError("No correct symbol");
		return false;
	}

	std::string function_name = "";

	function_name = GetToken().value;

	Shift(direction::next);

	auto& Save = _functions.back();
	Save.function_name = function_name;

	if (!parseDeclareFunction(Save.body_function))
		return false;

	Shift(direction::next);

	if (!matchCurrentToken(TTokenID::Colon))
	{
		if (ParseError) ParseError("No start declaration return type function");
		return false;
	}
	Shift(direction::next);
	if (!matchCurrentToken(TTokenID::Identifier))
	{
		if (ParseError) ParseError("No correct symbol");
		return false;
	}

	Save.return_type = GetToken().value;
	Shift(direction::next);
	return true;
};

bool ParserEngine::parseProcedure(std::vector<procedure_class>& _procedure) {

	// Пропускаем текущий токен
	Shift(direction::next);

	if (!matchCurrentToken(TTokenID::Identifier))
	{
		if (matchCurrentToken(TTokenID::LeftParen))
		{
			if (ParseError) ParseError("No find name procedure_class");
			return false;
		}
		if (ParseError) ParseError("No correct symbol");
		return false;
	}

	std::string function_name = "";

	function_name = GetToken().value;

	Shift(direction::next);

	auto& Save = _procedure.back();
	Save.procedure_name = function_name;

	if (!parseDeclareFunction(Save.body_function))
		return false;

	Shift(direction::next);
	return true;
};

bool ParserEngine::parseScope(int& TypeScope) {
	switch (GetToken().type)
	{
	case TTokenID::Public:
		TypeScope = Scope::Public;
		break;
	case TTokenID::Protected:
		TypeScope = Scope::Protect;
		break;
	case TTokenID::Private:
		TypeScope = Scope::Private;
		break;
	}
	return true;
};


bool ParserEngine::parseConstructor(constructor_class& constructor) {
	// Пропускаем текущий токен
	Shift(direction::next);

	if (!matchCurrentToken(TTokenID::Identifier))
	{
		if (matchCurrentToken(TTokenID::LeftParen))
		{
			if (ParseError) ParseError("No find name constructor_class");
			return false;
		}
		if (ParseError) ParseError("No correct symbol");
		return false;
	}

	std::string constructor_name = "";

	constructor_name = GetToken().value;
	
	Shift(direction::next);

	if (!parseDeclareFunction(constructor.body_function))
		return false;
	
	constructor.constructor_name = constructor_name;

	Shift(direction::next);
	return true;
};

bool ParserEngine::parseDestructor(destructor_class& destructor) {
	Shift(direction::next);

	if (!matchCurrentToken(TTokenID::Identifier))
	{
		if (matchCurrentToken(TTokenID::Semicolon))
		{
			if (ParseError) ParseError("No find name destructor_class");
			return false;
		}
		if (ParseError) ParseError("No correct symbol");
		return false;
	}

	std::string destructor_name = "";
	bool isOvveride = false;

	destructor_name = GetToken().value;

	Shift(direction::next);
	Shift(direction::next);

	if (isOvveride = matchCurrentToken(TTokenID::Override); isOvveride)
		Shift(direction::next);

	destructor.destructor_name = destructor_name;
	destructor.IsOvveride = isOvveride;

	return true;
};

bool ParserEngine::parseDeclareFunction(std::vector<BlockDeclareArgument>& Declare) {

	if (!matchCurrentToken(TTokenID::LeftParen))
	{
		if (ParseError) ParseError("No start declaration body function");
		return false;
	}

	BlockDeclareArgument CurrentDeclare;
	while (neof() && !matchCurrentToken(TTokenID::RightParen)) {
		
		if (IsQualifierToken())
		{
			if (!CurrentDeclare.Qualifier.empty())
			{
				if (ParseError) ParseError("Error reinitializing qualifier");
				return false;
			}
			CurrentDeclare.Qualifier = GetToken().value;
		}

		if (matchCurrentToken(TTokenID::Identifier))
			CurrentDeclare.Arguments.push_back(GetToken().value);

		if (matchCurrentToken(TTokenID::Colon))
		{
			Shift(direction::next);

			if (!matchCurrentToken(TTokenID::Identifier))
			{
				if (ParseError) ParseError("No type argument");
				return false;
			}

			CurrentDeclare.TypeArgument = GetToken().value;

			Shift(direction::next);

			if (matchCurrentToken(TTokenID::Equals))
			{
				if (CurrentDeclare.Arguments.size() > 2)
				{
					if (ParseError) ParseError("No argument more two");
					return false;
				}
				
				Shift(direction::next);
				CurrentDeclare.DefaultValue = GetToken().value;
			}
			
			Declare.push_back(CurrentDeclare);

			CurrentDeclare.Arguments.clear();
			CurrentDeclare.DefaultValue = "";
			CurrentDeclare.Qualifier = "";
			CurrentDeclare.TypeArgument = "";
			
			if (matchCurrentToken(TTokenID::RightParen))
				break;
		}
		Shift(direction::next);
	}

	return true;
};

void ParserEngine::ClearJunkToken(bool _direction) {
	
	if (_direction == direction::next) {
		while (neof() && JunkToken())
			PosBuffer++;
	}
	else {
		while (PosBuffer >= 0 && JunkToken())
			PosBuffer--;
	}
}

LexToken ParserEngine::GetToken() {
	return ParserBuffer[PosBuffer];
}

bool ParserEngine::matchCurrentToken(TTokenID kind) {
	return GetToken().type == kind;
}

bool ParserEngine::IsQualifierToken() {
	return 
		matchCurrentToken(TTokenID::Const) ||
		matchCurrentToken(TTokenID::Var) || 
		matchCurrentToken(TTokenID::Out);
}

bool ParserEngine::JunkToken() {
	return matchCurrentToken(TTokenID::LineFeed) || matchCurrentToken(TTokenID::Space);
}

std::vector<LexToken> ParserEngine::GetLexToken(int amount, const bool _direction)
{
	std::vector<LexToken> cache;
	cache.reserve(amount);
	for (int i = 0; i < amount; i++)
	{
		Shift(_direction);
		cache.emplace_back(GetToken());
	}
	return cache;
}

void ParserEngine::Shift(bool _direction) {
	_direction == direction::next ? PosBuffer++ : PosBuffer--;
	ClearJunkToken(_direction);
}