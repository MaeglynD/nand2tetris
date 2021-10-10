#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <experimental/filesystem>
#include <regex>
#include <cctype>
using namespace std;
namespace fs = std::experimental::filesystem;

string symbols = "{}()[].,;+-*/&|<>=-";
unordered_map<string, string> keywords = {
	{ "class", "" },
	{ "constructor", "" },
	{ "function", "" },
	{ "method", "" },
	{ "field", "" },
	{ "static", "" },
	{ "var", "" },
	{ "int", "" },
	{ "char", "" },
	{ "boolean", "" },
	{ "void", "" },
	{ "true", "" },
	{ "false", "" },
	{ "null", "" },
	{ "this", "" },
	{ "let", "" },
	{ "do", "" },
	{ "if", "" },
	{ "else", "" },
	{ "while", "" },
	{ "return", "" },
};

struct Token {
	string token;
	string type;
};

class CompilitationEngine {

	CompilitationEngine() {
		// 
	}

	void compileClass() {
		// 
	}

	void compileClassVarDec() {
		// 
	}

	void compileSubroutine(){ 

	}

	void compileParameterList() {
		// 
	}

	void compileVarDev() {
		// 
	}

	void compileStatements() {
		// 
	}

	void compileDo() {
		// 
	}

	void compileLet(){
		// 
	}

	void compileWhile() {
		// 
	}

	void compileReturn() {
		// 
	}

	void compileIf() {
		// 
	}

	void compileExpression() {
		// 
	}

	void compileTerm() {
		// 
	}

	void compileExpressionList() {
		// 
	}
};

class JackTokenizer {
	public:
		vector<Token> tokenVec;
		int currentTokenIndex = 0;

		JackTokenizer(ifstream& source) {
			string line;
			bool multiLineCommentActive = false;
			
			try {
				while(getline(source, line)) {
					while (!line.empty()) {
						char firstChar = line[0];
						string firstTwoChars = line.substr(0, 2);

						// Whitespace
						if (isspace(firstChar)) {
							line.erase(0, 1);
						}

						// During or at the end of a multi line comment
						else if (multiLineCommentActive) {
							if (firstTwoChars == "*/") {
								line.erase(0, 2);
								multiLineCommentActive = false;
							}
						}

						// Single line comment
						else if (firstTwoChars == "//") {
							break;
						}

						// Start of a multi line comment
						else if (firstTwoChars == "/*") {
							line.erase(0, 2);
							multiLineCommentActive = true;
						}

						// Int
						else if (isdigit(firstChar)) {
							// Ints range from 0 - 32767. Max amount of digits can be 5
							for (int i = 1; i < 6; i++) {
								if (!isdigit(line[i])) {
									pushAndErase(line, "INT_CONST", i - 1);
									break;
								}

								if (i > 5) {
									throw ("Large int: " + line);
								}
							}
						}

						// String
						else if (firstChar == '"') {
							int secondQuotationOccurrence = line.find_first_of(1, '"');

							if (secondQuotationOccurrence == string::npos) {
								throw ("String has no ending quotation: " + line);
							}
							
							pushAndErase(line, "STRING_CONST", secondQuotationOccurrence);
						}

						// Symbol
						else if (symbols.find_first_of(firstChar) != string::npos) {
							pushAndErase(line, "SYMBOL", 1);
						}

						// Keywords and identifiers
						else {
							bool hasFoundKeyword = false;

							// Keywords
							for(auto &keyword : keywords) {
								string kwa = keyword.first;
								int len = kwa.length();

								if (len <= line.length()) {
									string kwb = line.substr(0, len);

									// If the keyword is the same, indepedent of casing
									if (equal(kwa.begin(), kwa.end(), kwb.begin(), kwb.end(), [](char a, char b) {
										 return tolower(a) == tolower(b); 
									})) {
										hasFoundKeyword = true;
										pushAndErase(line, "KEYWORD", len);
									}
								}
							}

							// Identifiers
							if (!hasFoundKeyword) {
								int symbolPos = line.find_first_of(symbols);
								
								if (symbolPos != string::npos) {
									pushAndErase(line, "IDENTIFIER", symbolPos);
								} else {
									throw ("We got no idea how to translate this: " + line);
								}
							}
						}
					}
				}
			} catch (string errMsg) {
				cout << endl << "Tokenizer Error: " << errMsg;
			}
		}

		void pushAndErase(string &line, string type, int tokenLength) {
			tokenVec.push_back({ line.substr(0, tokenLength), type });
			line.erase(0, tokenLength);
		}

		bool hasMoreTokens() {
			return currentTokenIndex < tokenVec.size() - 1;
		}
		
		void advance() {
			currentTokenIndex++;
		}

		string tokenType() {
			return tokenVec[currentTokenIndex].type;
		}

		string currentToken() {
			return tokenVec[currentTokenIndex].token;
		}
};

class JackAnalyzer {
	public:
		JackAnalyzer(string userInput) {
			if (fs::is_directory(userInput)) {
				for (auto const& file : fs::directory_iterator(userInput)) {
					auto path = file.path();
					
					if (path.extension() == ".jack") {
						analyze("./" + userInput + "/" + path.stem().string());
					}
				}
			} else {
				analyze(userInput);
			}
		}

		void analyze(string name) {
			ofstream xmlFile(name + ".xml");
			ifstream jackFile(name + ".jack");

			JackTokenizer tokenizer(jackFile);
		}
};


int main(){
	string userInput;
	cin >> userInput;

	JackAnalyzer analyzer(userInput);

	return 0;
}
