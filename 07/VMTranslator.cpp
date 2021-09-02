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

unordered_map<string, string> arithmeticTable {
	{ 
		"add", R"(@SP
			M=M-1
			A=M
			D=M
			A=A-1
			M=D+M)" 
	},
	{ 
		"sub", R"(@SP
			M=M-1
			A=M
			D=M
			A=A-1
			M=M-D)"
	},
	{ 
		"neg", R"(@SP
			A=M-1
			M=-M)"
	},
	{ 
		"eq", R"(@SP
			M=M-1
			A=M
			D=M
			A=A-1
			D=D-M
			@EQ_TRUE_{id}
			D;JEQ
			@SP
			A=M-1
			M=0
			@EQ_END_{id}
			0;JMP
			(EQ_TRUE_{id})
			@SP
			A=M-1
			M=-1
			(EQ_END_{id}))" 
	},
	{ 
		"gt", R"(@SP
			M=M-1
			A=M
			D=M
			A=A-1
			D=M-D
			@GT_TRUE_{id}
			D;JGT
			@SP
			A=M-1
			M=0
			@GT_END_{id}
			0;JMP
			(GT_TRUE_{id})
			@SP
			A=M-1
			M=-1
			(GT_END_{id}))" 
	},
	{ 
		"lt", R"(@SP
			M=M-1
			A=M
			D=M
			A=A-1
			D=M-D
			@LT_TRUE_{id}
			D;JLT
			@SP
			A=M-1
			M=0
			@LT_END_{id}
			0;JMP
			(LT_TRUE_{id})
			@SP
			A=M-1
			M=-1
			(LT_END_{id}))" 
	},
	{ 
		"and", R"(@SP
			M=M-1
			A=M
			D=M
			A=A-1
			M=D&M)" 
	},
	{ 
		"or", R"(@SP
			M=M-1
			A=M
			D=M
			A=A-1
			M=D|M)"
	},
	{ 
		"not", R"(@SP
			A=M-1
			M=!M)" 
	},
};

class Parser {
	public:
		string command;
		int &identifier;
		ofstream &translatedFile;
		
		Parser(ofstream &file, string fileName, int &id) : translatedFile(file), identifier(id) {
			string line;
			ifstream assemblyFile(fileName);

			while(getline(assemblyFile, line)) {
				line = line.substr(0, line.find("//"));
				line.erase(0, line.find_first_not_of("\t\n\v\f\r "));
				line.erase(line.find_last_not_of("\t\n\v\f\r ") + 1);

				if (line != "") {
					command = line;
					string type = commandType();

					if (type == "C_ARITHMETIC") {
						writeArithmetic();
					}

				}
			}

			assemblyFile.close();
		}

		string getNthWord(int n) {
			string word = "";
			int delimCount = 0;

			for (int i = 0; i < command.size(); ++i) {
				if (command[i] == ' ') {
					delimCount++;
					
					if (delimCount == n) {
						return word;
					} else {
						word = "";
					}
				} else {
					word += command[i];
				}
			}
		}

		string arg1() {
			return getNthWord(2);
		}

		int arg2() {
			return stoi(getNthWord(3));
		}

		string commandType() {
			string firstWord = command.substr(0, command.find(" "));
			
			if (arithmeticTable.find(firstWord) != arithmeticTable.end()) {
				return "C_ARITHMETIC";
			}

			transform(firstWord.begin(), firstWord.end(), firstWord.begin(), ::toupper);

			return "C_" + firstWord;
		}

		void writeArithmetic() {
			translatedFile << regex_replace(command, regex("{id}"), to_string(identifier)) << endl;
		}
};

int main(){
	int identifier = 0;
	string userInput;
	cin >> userInput;
	
	ofstream translatedFile(userInput + ".hack");

	if (fs::is_directory(userInput)) {
		for (auto const& file : fs::directory_iterator(userInput)) {
			auto path = file.path();
			
			if (path.extension() == ".vm") {
				Parser parser(translatedFile, path.filename().string(), identifier);
			}
		}
	} else {
		Parser parser(translatedFile, userInput + ".vm", identifier);
	}

	translatedFile.close();

	cout << "Your .asm file is now ready. Enjoy.";
	
	return 0;
}
