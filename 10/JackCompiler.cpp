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
									pushAndErase(line, "integerConstant", i);
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
							
							pushAndErase(line, "stringConstant", secondQuotationOccurrence);
						}

						// Symbol
						else if (symbols.find_first_of(firstChar) != string::npos) {
							pushAndErase(line, "symbol", 1);
						}

						// Keywords and identifiers
						else {
							bool hasFoundKeyword = false;
							
							// Keywords
							for(auto &keyword : keywords) {
								string kwa = keyword.first;
								int len = kwa.length();

								if (len <= line.length() + 1) {
									string kwb = line.substr(0, len);
									bool isWordPresent = equal(kwa.begin(), kwa.end(), kwb.begin(), kwb.end(), [](char a, char b) {
										return a == tolower(b); 
									});

									// If the keyword is the same, indepedent of casing and if the subsequent character is a symbol
									// or a space. This accounts for cases such as var thisIsAVarName = ...; in which 'this' would
									// be extracted as its a keyword
									if (isWordPresent && (symbols + " ").find_first_of(line[len]) != string::npos) {
										hasFoundKeyword = true;
										pushAndErase(line, "keyword", len);
										
										break;
									}
								}
							}

							// Identifiers
							if (!hasFoundKeyword) {
								int symbolPos = line.find_first_of(symbols + " ");

								if (symbolPos != string::npos) {
									pushAndErase(line, "identifier", symbolPos);
								} else {
									throw ("We got no idea how to translate this: " + line);
								}
							}
						}
					}
				}
			} catch (const char* errMsg) {
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

			if (token == "<") {
				token = "&lt;";
			}
			
			if (token == ">") {
				token = "&gt;";
			}

			if (type == "stringConstant") {
				token = token.substr(1, token.size() - 2);
			}

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
			try {
				compileClass(false);
			} catch (const char* errMsg) {
				cout << endl << errMsg;
			}
		}

		void compileClass(bool printXML) {
			string contents = wrapInTags("class", 
				getContentsUntilSymbol("{")
					.append(getContents())
					.append(getContentsUntilSymbol("}"))
			);

			xmlFile << contents;
			xmlFile.close();

			if (printXML) {
				cout << contents;
			}

			cout << "\n You're XML file is now ready. Enjoy \n";
		}

		string getContentsUntilSymbol(string symbol, bool shouldIncludeStopper = true) {
			string contents;
			int failsafe = 0;

			// Include all tokens Up until the current token...
			while (!isCurrentToken(symbol)) {
				wrapAndAdvanceCurrentToken(contents);
				failsafe++;

				if (failsafe > 1000) {
					throw ("An overflow occured during the 'getContentsUntilSymbol' routine");
				}
			}

			if (shouldIncludeStopper) {
				wrapAndAdvanceCurrentToken(contents);
			}

			return contents;
		}

		void testTokenizerOutput() {
			for (auto &x : tokenizer.tokenVec) {
				xmlFile << "\n<" + x.type + "> " + x.token + " </" + x.type + ">"; 
			};
		}

		string getContents() {
			string contents;

			while (tokenizer.hasMoreTokens()) {
				if (isCurrentToken("}")) {
					wrapAndAdvanceCurrentToken(contents);
					break;
				}

				else if (isCurrentToken("field")) {
					contents += compileClassVarDec();
				}

				else if (isCurrentToken({ "method", "function", "constructor" })) {
					contents += compileSubroutine();
				}

				else if (isCurrentToken("var")) {
					contents += compileVarDec();
				}

				else if (isCurrentToken({ "if", "do", "let", "while", "return" })) {
					contents += compileStatements();
				}

				else {
					throw ("An error occurred during getContents(). CurrentToken cannot be parsed in this routine");
				}
			}

			return contents;
		}

		bool isCurrentToken(unordered_set<string> stringsToCompare) {
			return stringsToCompare.find(tokenizer.currentToken()) != stringsToCompare.end();
		}

		bool isCurrentToken(string stringToCompare) {
			return tokenizer.currentToken() == stringToCompare;
		}

		string wrapInTags (string tag, string innerText) {
			string possibleLineBreak = (innerText.size() > 0 && innerText[0] == '<') ? "\n" : "";

			return "<" + tag + "> " + possibleLineBreak + innerText + " </" + tag + ">\n";
		}

		string wrapAndAdvanceCurrentToken(string &contents) {
			contents += wrapInTags(tokenizer.currentType(), tokenizer.currentToken());
			tokenizer.advance();
			
			return contents;
		}

		string compileClassVarDec() {
			return wrapInTags("classVarDec", getContentsUntilSymbol(";"));
		}

		string compileSubroutine(){
			string contents = getContentsUntilSymbol("(")
				.append(compileParameterList())
				.append(getContentsUntilSymbol(")"))
				.append(
					wrapInTags("subroutineBody", getContentsUntilSymbol("{").append(getContents()))
				);

				return wrapInTags("subroutineDec", contents);
		}

		string compileParameterList() {
			return wrapInTags("parameterList", getContentsUntilSymbol(")", false));
		}

		string compileVarDec() {
			return wrapInTags("varDec", getContentsUntilSymbol(";"));
		}

		string compileStatements() {
			string contents; 
			int failsafe = 0;
			unordered_set<string> conditions = { "if", "do", "let", "while", "return" };

			// If theres no statements within the block
			if (isCurrentToken("}")) {
				wrapAndAdvanceCurrentToken(contents);
				return contents;
			}

			if (!isCurrentToken(conditions)) {
				throw ("An error occured in compileStatements routine: routine called without keyword being a statement");
			}
				
			while (isCurrentToken(conditions)) {
				contents += compileStatement();

				failsafe++;
				
				if (failsafe > 1000) {
					throw ("An overflow occured during the compileStatements routine.");
				}
			}

			contents = wrapInTags("statements", contents);

			return contents;
		}

		string compileStatement(){
			string contents;
			string activeStatement = tokenizer.currentToken();

			if (isCurrentToken(unordered_set<string>({ "if", "while" }))) {
				contents = getContentsUntilSymbol("(")
					.append(compileExpression({ ")" }))
					.append(getContentsUntilSymbol("{"))
					.append(compileStatements());

				wrapAndAdvanceCurrentToken(contents);
			}

			else if (isCurrentToken("let")) {
				wrapAndAdvanceCurrentToken(contents);
				wrapAndAdvanceCurrentToken(contents);

				if (isCurrentToken("[")) {
					contents += compileExpression({ "]" });
				}

				contents += getContentsUntilSymbol("=")
					.append(compileExpression());
			}

			else if (isCurrentToken("do")) {
				contents = getContentsUntilSymbol("(")
					.append(compileExpressionList()) 
					.append(getContentsUntilSymbol(";"));
			}

			else if (isCurrentToken("return")) {
				wrapAndAdvanceCurrentToken(contents);
				contents += compileExpression();
			}

			else {
				throw ("compileStatement was called but no statement was parsed");
			}

			return wrapInTags(activeStatement + "Statement", contents);
		}

		
		string compileExpression(unordered_set<string> stoppingTokens = { ";" }, bool shouldIncludeStopper = true) {
			string contents;

			if (isCurrentToken(";")) {
				wrapAndAdvanceCurrentToken(contents);
			} else {

				while (!isCurrentToken(stoppingTokens)) {
					string type = tokenizer.currentType();
					bool isTermable = (
						type == "identifier" || 
						type == "integerConstant" || 
						type == "stringConstant" ||
						type == "keyword" ||
						isCurrentToken({ "(", "~", "-" })
					);
					
					if (isTermable) {
						contents += compileTerm();
					} else {
						wrapAndAdvanceCurrentToken(contents);
					}
				}

				contents = wrapInTags("expression", contents);

				if (shouldIncludeStopper) {
					wrapAndAdvanceCurrentToken(contents);
				}
			}

			return contents;
		}

		string compileTerm() {
			string contents;
			string prevType = tokenizer.currentType();
			string prevToken = tokenizer.currentToken();

			wrapAndAdvanceCurrentToken(contents);

			if (prevToken == "(") {
				contents += compileExpression({ ")" });
			}

			else if (prevType == "identifier") {
				if (isCurrentToken("(")) {
					contents += compileExpression({ ")" });
				}

				else if (isCurrentToken("[")) {
					contents += compileExpression({ "]" });
				}

				else if (isCurrentToken(".")) {
					contents += getContentsUntilSymbol("(").append(compileExpressionList());
				}
			}

			else if (prevType == "symbol") {
				contents += compileTerm();
			}

			return wrapInTags("term", contents);
		}

		string compileExpressionList() {
			string contents;

			while (!isCurrentToken(")")) {
				if (isCurrentToken(",")) {
					wrapAndAdvanceCurrentToken(contents);
				} else {
					contents += compileExpression({ ",", ")" }, false);
				}
			}

			contents = wrapInTags("expressionList", contents);
			wrapAndAdvanceCurrentToken(contents);

			return contents;
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
