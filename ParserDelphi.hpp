

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

namespace direction {
	constexpr bool next = true;
	constexpr bool previous = false;
}

class ParserEngine {
	
	int PosBuffer = 0;

	std::vector<LexToken> ParserBuffer;
	std::vector<std::pair<int, AST*>> AstBuffer;
	
	
	void ClearJunkToken(bool _direction);
	LexToken GetCleanToken(bool SafePosition, bool _direction);
	bool matchDierctionToken(bool _direction, TTokenID kind);
	bool matchCurrentToken(TTokenID kind);
	bool JunkToken();

	std::vector<LexToken> GetLexToken(
		int amount, 
		bool _direction
	);

	bool MatchPattern(
		std::vector<TTokenID>& tokens,
		bool _direction
	);

	bool parseUnit();
	bool parseClass();

public:
	ParserEngine(std::vector<LexToken> Buffer) { ParserBuffer = Buffer; };
	~ParserEngine() { for (auto& [left, right] : AstBuffer) delete right; };
	void ParserRun();
};

void ParserEngine::ParserRun() {
	while (PosBuffer < ParserBuffer.size())
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

	while (PosBuffer < ParserBuffer.size() && !matchCurrentToken(TTokenID::Semicolon)) {
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

	std::string programm_name = buffer[1].value;
	PosBuffer++;
	
	// Парсии тело акта
	while (PosBuffer < ParserBuffer.size() && !matchCurrentToken(TTokenID::End)) {
		PosBuffer++;
		ClearJunkToken(direction::next);

		
	};

	return true;
};

void ParserEngine::ClearJunkToken(bool _direction) {
	
	if (_direction == direction::next) {
		while (PosBuffer < ParserBuffer.size() && JunkToken())
			PosBuffer++;
	}
	else {
		while (PosBuffer >= 0 && JunkToken())
			PosBuffer--;
	}
}

LexToken ParserEngine::GetCleanToken(bool SafePosition, bool _direction) {
	
	int oldPos = PosBuffer;
	_direction == direction::next ? PosBuffer++ : PosBuffer--;
	ClearJunkToken(_direction);
	LexToken token = ParserBuffer[PosBuffer];
	if (SafePosition)
		PosBuffer = oldPos; // Восстанавливаем позицию
	return token;
}

bool ParserEngine::matchDierctionToken(bool _direction, TTokenID kind) {
	int oldPos = PosBuffer;
	_direction == direction::next ? PosBuffer++ : PosBuffer--;
	ClearJunkToken(_direction);
	bool result = matchCurrentToken(kind);
	PosBuffer = oldPos; // Восстанавливаем позицию
	return result;
}

bool ParserEngine::matchCurrentToken(TTokenID kind) {
	return ParserBuffer[PosBuffer].type == kind;
}

bool ParserEngine::JunkToken() {
	return matchCurrentToken(TTokenID::LineFeed) || matchCurrentToken(TTokenID::Space);
}

bool ParserEngine::MatchPattern(std::vector<TTokenID>& tokens, bool _direction) 
{	
	_direction == direction::next ? PosBuffer++ : PosBuffer--;
	while (!tokens.empty()) {
		TTokenID TypeToken;
		ClearJunkToken(_direction);
		if (_direction == direction::next) {
			TypeToken = tokens.front();
			if (!matchCurrentToken(TypeToken))
				return false;
			tokens.erase(tokens.begin());
			PosBuffer++;
		}
		else {
			TypeToken = tokens.back();
			if (!matchCurrentToken(TypeToken))
				return false;
			tokens.pop_back();
			PosBuffer--;
		}
	}
	return true;
}

std::vector<LexToken> ParserEngine::GetLexToken(int amount, const bool _direction)
{
	std::vector<LexToken> cache;
	cache.reserve(amount);
	for (int i = 0; i < amount; i++)
		cache.emplace_back(GetCleanToken(false, _direction));
	return cache;
}