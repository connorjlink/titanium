#include "lexer.hpp"

#include <array>
#include <string>

#include <cctype>
#include <cassert>

namespace
{
	static constexpr auto my_isalpha = [](char c)
	{
		return std::isalpha(static_cast<unsigned char>(c));
	};

	static constexpr auto my_isalnum = [](char c)
	{
		return std::isalnum(static_cast<unsigned char>(c));
	};

	static constexpr auto my_isdigit = [](char c)
	{
		return std::isdigit(static_cast<unsigned char>(c));
	};
}

namespace ti
{
	Lexer::Lexer(std::string data) noexcept
		: data(data), cursor(0)
	{

	}

	std::vector<Token> Lexer::Lex(void) noexcept
	{
		std::vector<Token> tokens;

		while (!AtEnd())
		{
			if (Token token; Scan(token))
			{
				tokens.emplace_back(token);
			}
		}

		return tokens;
	}


	char Lexer::Current(void) noexcept
	{
		return data[cursor];
	}

	char Lexer::Next(void) noexcept
	{
		return data[cursor + (AtEnd() ? 1 : 0)];
	}

	char Lexer::Advance(void) noexcept
	{
		if (!AtEnd())
			cursor++;

		return Current();
	}

	bool Lexer::AtEnd(void) noexcept
	{
		return (cursor == data.length());
	}


	std::string Lexer::Rest(std::function<int(int)> functor = my_isalpha) noexcept
	{
		std::string out;

		while (!AtEnd() && functor(Current()))
		{
			out += Current();
			Advance();
		}

		return out;
	}

	bool Lexer::Scan(Token& out) noexcept
	{
		using enum Token::TokenType;
		using enum Symbol::SymbolType;
		using enum Keyword::KeywordType;
		switch (Current())
		{
			case '-':
			{
				if (Next() == '-') 
				{ 
					out.type = SYMBOL; 
					out.as.symbol.type = MINUS_MINUS;
					Advance(); 
				}

				else
				{
					out.type = SYMBOL;
					out.as.symbol.type = MINUS;
				}
			} break;

			case '+':
			{
				if (Next() == '+') 
				{ 
					out.type = SYMBOL;
					out.as.symbol.type = PLUS_PLUS;
					Advance();
				}

				else
				{
					out.type = SYMBOL;
					out.as.symbol.type = PLUS;
				}
			} break;

			case '|':
			{
				out.type = SYMBOL;
				out.as.symbol.type = OR;
			} break;

			case '&':
			{
				out.type = SYMBOL;
				out.as.symbol.type = AND;
			} break;

			case '<':
			{
				if (Next() == '<')
				{ 
					out.type = SYMBOL;
					out.as.symbol.type = LSHIFT;
					Advance();
				}

				else
				{
					out.type = SYMBOL;
					out.as.symbol.type = LESS;
				}
			} break;

			case '>':
			{
				if (Next() == '>')
				{
					out.type = SYMBOL;
					out.as.symbol.type = RSHIFT;
					Advance();
				}

				else
				{
					out.type = SYMBOL;
					out.as.symbol.type = GREATER;
				}
			} break;

			case '!':
			{
				assert(Next() == '=');
				out.type = SYMBOL;
				out.as.symbol.type = NOT_EQ;
			} break;

			case '=':
			{
				if (Next() == '=') 
				{ 
					out.type = SYMBOL;
					out.as.symbol.type = EQ_EQ;
					Advance(); 
				}

				else
				{
					out.type = SYMBOL;
					out.as.symbol.type = EQ;
				}
			} break;

			case ';':
			{
				out.type = SYMBOL;
				out.as.symbol.type = SEMICOLON;
			} break;

			case ',':
			{
				out.type = SYMBOL;
				out.as.symbol.type = COMMA;
			} break;

			case '(':
			{
				out.type = SYMBOL;
				out.as.symbol.type = LPAREN;
			} break;

			case ')':
			{
				out.type = SYMBOL;
				out.as.symbol.type = RPAREN;
			} break;

			case '{':
			{
				out.type = SYMBOL;
				out.as.symbol.type = LBRACE;
			} break;

			case '}':
			{
				out.type = SYMBOL;
				out.as.symbol.type = RBRACE;
			} break;

			case 'f':
			{
				auto rest = Rest();
				assert(rest == "function");
				out.type = KEYWORD;
				out.as.keyword.type = FUNCTION;
			} break;

			case 'p':
			{
				auto rest = Rest();
				assert(rest == "proto");
				out.type = KEYWORD;
				out.as.keyword.type = PROTO;
			} break;

			case 'v':
			{
				auto rest = Rest();
				out.type = KEYWORD;

					 if (rest == "val")  out.as.keyword.type = VAL;
				else if (rest == "void") out.as.keyword.type = VOID;
				else				     assert(false);
			} break;

			case 'b':
			{
				auto rest = Rest();
				out.type = KEYWORD;

					 if (rest == "byte") out.as.keyword.type = BYTE;
				else if (rest == "bool") out.as.keyword.type = BYTE;
				else					assert(false);
			} break;

			case 'l':
			{
				auto rest = Rest();
				assert(rest == "local");

				out.type = KEYWORD;
				out.as.keyword.type = LOCAL;
			} break;

			case 'g':
			{
				auto rest = Rest();
				assert(rest == "global");

				out.type = KEYWORD;
				out.as.keyword.type = GLOBAL;
			} break;

			case 'i':
			{
				auto rest = Rest();
				assert(rest == "if");

				out.type = KEYWORD;
				out.as.keyword.type = IF;
			} break;

			case 'w':
			{
				auto rest = Rest();
				assert(rest == "while");

				out.type = KEYWORD;
				out.as.keyword.type = WHILE;
			} break;

			case 'r':
			{
				auto rest = Rest();
				assert(rest == "return");

				out.type = KEYWORD;
				out.as.keyword.type = RETURN;
			} break;

			case ' ': [[fallthrough]];
			case '\t': [[fallthrough]];
			case '\n':
			{
				Advance();
				return false;
			} break;

			default:
			{
				if (my_isalpha(Current()))
				{
					auto rest = Rest(my_isalnum);
					out.type = IDENTIFIER;

					out.as.identifier.name = new char[rest.length()];
					std::copy(rest.begin(), rest.end(), out.as.identifier.name);
					out.as.identifier.name[rest.length()] = '\0';
					return true;
				}

				else if (my_isdigit(Current()))
				{
					auto rest = Rest(my_isdigit);
					out.type = NUMBER;
					out.as.number.value = std::stoi(rest);
					return true;
				}

				assert(false);
			} break;
		}

		Advance();

		return true;
	}
}
