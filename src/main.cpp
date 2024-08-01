#include "lexer/Lexer.hpp"
#include "parser/Parser.hpp"
#include "token/Token.hpp"
#include "semantic/SemanticAnalyzer.hpp"

#include <iostream>
#include <vector>

int main(int argc, char* argv[])
{
	std::string code = "n = 5\n fact=1\n while(n>0)\n{\nfact *= n\nn--\n}\nprint fact\n ++a";

	kmsl::Lexer lexer(code);
	std::vector<kmsl::Token> tokens = lexer.scanTokens();

	for (const kmsl::Token& token : tokens)
		std::cout << "Type: " << (int)token.type << " Text: " << token.text << " Pos: " << token.pos << std::endl;

	kmsl::Parser parser(tokens);
	std::unique_ptr<kmsl::BlockNode> root = parser.parse();
	std::cout << std::endl << root->toString() << std::endl;

	kmsl::SemanticAnalyzer semantic;
	semantic.analyze(root);

	return 0;
}