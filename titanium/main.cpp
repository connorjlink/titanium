#include <cstdlib>
#include <iostream>
#include <fstream>
#include <filesystem>

#include "lexer.hpp"

int main(int argc, char** argv)
{
	if (argc == 2)
	{
		std::string filepath = argv[1];
		auto size = std::filesystem::file_size(filepath);
		std::string data('\0', size);
		
		std::ifstream file(filepath, std::ios::in);

		data.assign((std::istreambuf_iterator<char>(file)),
					 std::istreambuf_iterator<char>());


		ti::Lexer lexer{ data };
		auto tokens = lexer.Lex();
		for (auto& token : tokens)
		{
			std::cout << (int)token.type << std::endl;
		}

		return EXIT_SUCCESS;
	}

	else
	{
		return EXIT_FAILURE;
	}
}