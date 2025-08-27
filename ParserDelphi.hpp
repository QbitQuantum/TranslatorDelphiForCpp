

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


	int PosBuffer = 0;

	std::vector<LexToken> ParserBuffer;
	std::vector<std::pair<int, AST*>> AstBuffer;
	
	void Shift(bool _direction);
	
	void ClearJunkToken(bool _direction);
	
	bool matchCurrentToken(TTokenID kind);
	bool JunkToken();
	bool neof() {
		return PosBuffer < ParserBuffer.size();
	}

	LexToken GetToken();

	std::vector<LexToken> GetLexToken(
		int amount, 
		bool _direction
	);

	bool parseUnit();
	bool parseClass();
	bool parseProperty(std::vector<property_method>& _property_method);
	bool parseScope(int& TypeScope);
	bool parseConstructor(int TypeScope);
	bool parseDestructor(int TypeScope);

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
		PosBuffer++;
		ClearJunkToken(direction::next);
	}
}

bool ParserEngine::parseUnit() {
	PosBuffer++; // Пропускаем 'unit'
	ClearJunkToken(direction::next);
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

	class_name = buffer[1].value;
	PosBuffer++;
	
	ClearJunkToken(direction::next);

	// Проверяем, наследуются ли другие классы
	if (matchCurrentToken(TTokenID::LeftParen))
	{
		while (neof() && !matchCurrentToken(TTokenID::RightParen)) {
			PosBuffer++;
			ClearJunkToken(direction::next);
			if (matchCurrentToken(TTokenID::Identifier))
				legatee.push_back(ParserBuffer[PosBuffer].value);
		}
	}

	// Пропускаем RightParen
	PosBuffer++;

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
			break;
		case TTokenID::Constructor:
			if (!parseConstructor(TypeScope))
				return false;
			break;
		case TTokenID::Destructor:
			if (!parseDestructor(TypeScope))
				return false;
			break;
		case TTokenID::Property:
			property_methods.push_back({});
			property_methods.back().TypeScope = TypeScope;
			if (!parseProperty(property_methods))
				return false;
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
		}
			break;
		default:
			break;
		}

		PosBuffer++;
		ClearJunkToken(direction::next);
	}
	return true;
};

bool ParserEngine::parseProperty(std::vector<property_method>& _property_method) {
	
	std::string property_name = "";
	std::string property_type = "";
	std::string property_read_name = "";
	std::string property_write_name = "";

	//Пропускаем текущий токен
	PosBuffer++;
	ClearJunkToken(direction::next);

	if (!matchCurrentToken(TTokenID::Identifier))
	{
		if (ParseError) ParseError("No find name property"); return false;
	}
	property_name = GetToken().value;
	
	PosBuffer++;
	ClearJunkToken(direction::next);

	if (!matchCurrentToken(TTokenID::Colon))
	{
		if (ParseError) ParseError("No find start declaration method property"); return false;
	}

	PosBuffer++;
	ClearJunkToken(direction::next);

	if (!matchCurrentToken(TTokenID::Identifier))
	{
		if (ParseError) ParseError("No find type property method"); return false;
	}

	property_type = GetToken().value;


	while (neof() && !matchCurrentToken(TTokenID::Semicolon)) {
		if (matchCurrentToken(TTokenID::Read))
		{
			PosBuffer++;
			ClearJunkToken(direction::next);
			if (!matchCurrentToken(TTokenID::Identifier))
			{
				if (ParseError) ParseError("No correct name read property"); return false;
			}
			property_read_name = GetToken().value;
		}

		if (matchCurrentToken(TTokenID::Write))
		{
			PosBuffer++;
			ClearJunkToken(direction::next);
			if (!matchCurrentToken(TTokenID::Identifier))
			{
				if (ParseError) ParseError("No correct name write property"); return false;
			}
			property_write_name = GetToken().value;
		}
		PosBuffer++;
	}

	auto& Save = _property_method.back();
	Save.name_method = property_name;
	Save.type_method = property_type;
	Save.name_read = property_read_name;
	Save.name_write = property_write_name;
	return true;
};

bool ParserEngine::parseScope(int& TypeScope) {
	switch (GetToken().type)
	{
	case TTokenID::Public:
		TypeScope = Scope::Public;
		PosBuffer++;
		break;
	case TTokenID::Protected:
		TypeScope = Scope::Protect;
		PosBuffer++;
		break;
	case TTokenID::Private:
		TypeScope = Scope::Private;
		PosBuffer++;
		break;
	}
	return true;
};


bool ParserEngine::parseConstructor(int TypeScope) {
	// Заглушка
	std::string constructor_name = "";

	Shift(direction::next);

	if (!matchCurrentToken(TTokenID::Identifier))
		return false;

	constructor_name = ParserBuffer[PosBuffer].value;
	
	Shift(direction::next);

	if (!matchCurrentToken(TTokenID::LeftParen))
		return false;

	while (neof() && !matchCurrentToken(TTokenID::RightParen)) {
		PosBuffer++;
	}
	return true;
};

bool ParserEngine::parseDestructor(int TypeScope) {
	// Получаем деструктор 
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
	return ParserBuffer[PosBuffer].type == kind;
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