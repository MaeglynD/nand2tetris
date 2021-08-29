#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <experimental/filesystem>
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
		vector<string> commands;

		Parser(ofstream &translatedFile, string fileName) {
			string line;
			ifstream assemblyFile(fileName);

			while(getline(assemblyFile, line)) {
				line = line.substr(0, line.find("//"));
				line.erase(0, line.find_first_not_of("\t\n\v\f\r "));
				line.erase(line.find_last_not_of("\t\n\v\f\r ") + 1);

				if (line != "") {
					commands.push_back(line);
				}
			}

			assemblyFile.close();
		}

		string commandType(string command) {
			string firstWord = command.substr(0, command.find(" "));

			if (firstWord == "call") {
				return "C_CALL";
			}
			
			if (firstWord == "return") {
				return "C_RETURN";
			}

			if (firstWord == "function") {
				return "C_FUNCTION";
			}

			if (firstWord == "goto") {
				return "C_GOTO";
			}

			if (firstWord == "if-goto") {
				return "C_IF";
			}

			return "";
		}
};


int main(){
	string userInput;
	cin >> userInput;
	ofstream translatedFile(userInput + ".hack");

	if (fs::is_directory(userInput)) {
		for (auto const& file : fs::directory_iterator(userInput)) {
			auto path = file.path();
			
			if (path.extension() == ".vm") {
				Parser parser(translatedFile, path.filename().string());
			}
		}
	} else {
		Parser parser(translatedFile, userInput + ".vm");
	}

	translatedFile.close();

	cout << "Your .asm file is now ready. Enjoy.";
	
	return 0;
}
