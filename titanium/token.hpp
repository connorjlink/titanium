#ifndef TOKEN_HPP
#define TOKEN_HPP

namespace ti
{
	struct Symbol
	{
		enum class SymbolType
		{
			MINUS_MINUS, PLUS_PLUS,
			MINUS, PLUS,

			OR, AND,

			LSHIFT, RSHIFT,

			LESS, GREATER,

			NOT_EQ, EQ_EQ,

			SEMICOLON, COMMA, EQ,

			LPAREN, RPAREN, LBRACE, RBRACE,
		} type;
	};

	struct Keyword
	{
		enum class KeywordType
		{
			FUNCTION,
			PROTO,
			VAL,
			VOID,
			BYTE,
			BOOL,
			LOCAL,
			GLOBAL,
			IF,
			WHILE,
			RETURN,
		} type;
	};

	struct Identifier
	{
		char* name;
	};

	struct Number
	{
		std::uint8_t value;
	};

	struct Token
	{
		enum class TokenType
		{
			SYMBOL,
			KEYWORD,

			IDENTIFIER,
			NUMBER,
		} type;

		union Value
		{
			Symbol symbol;
			Keyword keyword;
			Identifier identifier;
			Number number;
		} as;
	};
}

#endif