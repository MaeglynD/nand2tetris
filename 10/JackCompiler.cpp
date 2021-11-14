#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <experimental/filesystem>
#include <regex>
#include <cctype>
using namespace std;
namespace fs = std::experimental::filesystem;

string symbols = "{}()[].,;+-*/&|<>=~";
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

class JackTokenizer {
	public:
		vector<Token> tokenVec;
		int currentTokenIndex = 0;

		JackTokenizer(ifstream &source) {
			string line;
			bool multiLineCommentActive = false;
			
			try {
				while(getline(source, line)) {
					while (!line.empty()) {
						char firstChar = line[0];
						string firstTwoChars = line.substr(0, line.length() > 1 ? 2 : 1);

						// Multi line comment
						if (multiLineCommentActive) {
							int pos = line.find("*/");

							if (pos != string::npos) {
								line.erase(0, pos + 3);
								multiLineCommentActive = false;
							} else {
								// Go to next line
								break;
							}
						}

						// Whitespace
						else if (isspace(firstChar)) {
							// TODO: positive lookahead remove further whitespace
							line.erase(0, 1);
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
									pushAndErase(line, "INT_CONST", i);
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
										return a == tolower(b); 
									})) {
										hasFoundKeyword = true;
										pushAndErase(line, "KEYWORD", len);
										break;
									}
								}
							}

							// Identifiers
							if (!hasFoundKeyword) {
								int symbolPos = line.find_first_of(symbols + " ");

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

		void pushAndErase(string &line, string type, int tokenLength = -1) {
			// Often we simply want to split by whitespace. We ensure this by defining
			// tokenLength as -1
			if (tokenLength == -1) {
				tokenLength = (unsigned int)line.find_first_of(" \t\n\v\f\r");
			}

			string token = line.substr(0, tokenLength);
			tokenVec.push_back({ token, type });
			line.erase(0, tokenLength);
		}

		bool hasMoreTokens() {
			return currentTokenIndex < tokenVec.size() - 1;
		}
		
		void advance() {
			currentTokenIndex++;
		}

		string currentType() {
			return tokenVec[currentTokenIndex].type;
		}

		string currentToken() {
			return tokenVec[currentTokenIndex].token;
		}
};

class CompilationEngine {
	public:
		ofstream &xmlFile;
		JackTokenizer &tokenizer;

		CompilationEngine(ofstream &_xmlFile, JackTokenizer &_tokenizer) : xmlFile(_xmlFile), tokenizer(_tokenizer) {
			compileClass();
		}

		void testTokenizerOutput() {
			for (auto &x : tokenizer.tokenVec) {
				xmlFile << "\n<" + x.type + "> " + x.token + " </" + x.type + ">"; 
			};
		}

		string getContentsRecursively(bool stopAtEndOfBlock = false) {
			string contents;

			if (stopAtEndOfBlock && tokenizer.currentToken() == "}") {
				return;
			}

			if (isCurrentToken("field")) {
				contents += compileClassVarDec();
			}

			if (isCurrentToken({ "method", "function", "constructor" })) {
				contents += compileSubroutine();
			}

			if (isCurrentToken("var")) {
				contents += compileVarDec();
			}

			if (isCurrentToken({ "if", "do", "let", "while", "return" })) {
				contents += compileStatements();
			}

			tokenizer.advance();

			return tokenizer.hasMoreTokens() ? getContentsRecursively(stopAtEndOfBlock) : contents;
		}

		bool isCurrentToken(unordered_set<string> stringsToCompare) {
			return stringsToCompare.find(tokenizer.currentToken()) != stringsToCompare.end();
		}

		bool isCurrentToken(string stringToCompare) {
			return tokenizer.currentToken() == stringToCompare;
		}

		string wrapInTags (string tag, string innerText) {
			return "<" + tag + ">\n" + innerText + "\n</" + tag + ">";
		}

		void compileClass() {
			xmlFile << wrapInTags("class", getContentsUntilSymbol("{") + getContentsRecursively(true));
		}

		string wrapAndAdvanceCurrentToken(string &contents) {
			contents += wrapInTags(tokenizer.currentType(), tokenizer.currentToken());
			tokenizer.advance();
			
			return contents;
		}

		string getContentsUntilSymbol(string symbol) {
			string contents;
			int failsafe = 0;

			while (!isCurrentToken(symbol)) {
				wrapAndAdvanceCurrentToken(contents);
				failsafe++;

				if (failsafe > 1000) {
					throw ("An overflow occured during the 'getContentsUntilSymbol' routine");
				}
			}

			wrapAndAdvanceCurrentToken(contents);

			return contents;
		}

		string compileClassVarDec() {
			return wrapInTags("classVarDec", getContentsUntilSymbol(";"));
		}

		string compileSubroutine(){
			return wrapInTags(
				"subroutine", 
				getContentsUntilSymbol("(")
					+ compileParameterList() 
					+ getContentsUntilSymbol("{") 
					+ wrapInTags("subroutineBody", getContentsRecursively(true))
			);
		}

		string compileParameterList() {
			return wrapInTags("parameterList", getContentsUntilSymbol(")"));
		}

		string compileVarDec() {
			return wrapInTags("varDec", getContentsUntilSymbol(";"));
		}

		string compileStatements() {
			string contents; 
			unordered_set<string> conditions = { "if", "do", "let", "while", "return" };

			if (isCurrentToken("}")) {
				wrapAndAdvanceCurrentToken(contents);
			} else {
				if (!isCurrentToken(conditions)) {
					throw ("An error occured in compileStatements routine: routine called without keyword being a statement");
				}
					
				while (isCurrentToken(conditions)) {
					contents += compileStatement();
				}
			}

			return wrapInTags("statements", contents);
		}

		string compileStatement(){
			string contents;

			if (isCurrentToken({ "if", "while" })) {
				getContentsUntilSymbol("(");
				compileExpression();
			}

			return wrapInTags(tokenizer.currentToken() + "Statement", contents);
		}

		
		string compileExpression(string stoppingToken) {
			string contents;

			if (isCurrentToken(";")) {
				wrapAndAdvanceCurrentToken(contents);
			} else {
				while (!isCurrentToken(stoppingToken)) {
					contents += compileTerm();
				}
			}

			return contents;
		}

		string compileDo() {
			// 
		}

		string compileLet(){
			// 
		}

		string compileWhile() {
			// 
		}

		string compileReturn() {
			// 
		}

		string compileIf() {
			// 
		}

		string compileTerm() {
			string prevToken = tokenizer.currentToken();

			tokenizer.advance();
		}

		string compileExpressionList() {
			// 
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
			ofstream xmlFile(name + "T.xml");
			ifstream jackFile(name + ".jack");

			JackTokenizer tokenizer(jackFile);
			CompilationEngine compEngine(xmlFile, tokenizer);

			xmlFile.close();
			jackFile.close();
		}
};


int main(){
	string userInput;
	cin >> userInput;

	JackAnalyzer analyzer(userInput);

	return 0;
}
