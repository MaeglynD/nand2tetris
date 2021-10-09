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
		vector<string> tokenVec;
		
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
							continue;
						}

						// Single line comment
						if (firstTwoChars == "//") {
							break;
						}

						// During or at the end of a multi line comment
						if (multiLineCommentActive) {
							if (firstTwoChars == "*/") {
								line.erase(0, 2);
								multiLineCommentActive = false;
							}

							continue;
						}

						// Start of a multi line comment
						if (firstTwoChars == "/*") {
							line.erase(0, 2);
							multiLineCommentActive = true;
							continue;
						}

						// Int
						if (isdigit(firstChar)) {
							// Ints range from 0 - 32767. Max amount of digits can be 5
							for (int i = 1; i < 6; i++) {
								if (!isdigit(line[i])) {
									tokenVec.push_back(line.substr(0, i - 1));
									line.erase(0, i - 1);
									break;
								}

								if (i > 5) {
									throw ("Large int");
								}
							}

							continue;
						}

						// String
						if (firstChar == '"') {
							int secondQuotationOccurrence = line.find_first_of(1, '"');

							if (secondQuotationOccurrence == string::npos) {
								throw ("String has no ending quotation");
							}
							
							tokenVec.push_back(line.substr(0, secondQuotationOccurrence));
							line.erase(0, secondQuotationOccurrence);

							continue;
						}

						// Symbol
						if (symbols.find_first_of(firstChar) != string::npos) {
							tokenVec.push_back(string(1, firstChar));
							line.erase(0, 1);
							continue;
						}

						// Keywords
					}
				}
			} catch (string errMsg) {
				cout << endl << "Tokenizer Error: " << errMsg;
			}
		}

		bool hasMoreTokens() {
			return false;
		}
		
		void advance() {
			// 
		}

		string tokenType() {
			return "";
		}

		string keyWord() {
			return "";
		}

		char symbol() {
			return 'A';
		}

		string identifier() {
			return "";
		}

		int intVal() {
			return 0;
		}

		string stringVal() {
			return "";
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
