#ifndef LEXER_HPP
#define LEXER_HPP

#include <functional>
#include <vector>
#include <string>

#include <cstdint>

#include "token.hpp"

namespace ti
{
	class Lexer
	{
	private:
		std::string data;
		std::int32_t cursor;

	public:
		Lexer(std::string) noexcept;

		std::vector<Token> Lex(void) noexcept;

	private:
		char Current(void) noexcept;
		char Next(void) noexcept;

		bool AtEnd(void) noexcept;

		//additionally returns the new current
		char Advance(void) noexcept;

		std::string Rest(std::function<int(int)>) noexcept;

		//finds a single next token
		bool Scan(Token& out) noexcept;
	};
}

#endif
