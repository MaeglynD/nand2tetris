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

unordered_map<string, string> argumentTable {
	{ "local", "LCL" },
	{ "argument", "ARG" },
	{ "this", "THIS" },
	{ "that", "THAT" },
	{ "pointer", "THIS" },
	{ "temp", "5" },
	{ "static", "" },
	{ "constant", "" },
};

class Parser {
	public:
		string command;
		string name;
		int &identifier;
		ofstream &translatedFile;
		
		Parser(ofstream &file, string fileName, int &id) : translatedFile(file), identifier(id), name(fileName) {
			string line;
			ifstream assemblyFile(name + ".vm");

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

					if (type == "C_PUSH" || type == "C_POP") {
						writePushPop(type);
					}

					if (type == "C_LABEL") {
						// 
					}

					if (type == "C_GOTO") {
						// 
					}

					if (type == "C_IF-GOTO") {
						// 
					}

					if (type == "C_FUNCTION") {
						// 
					}

					if (type == "C_RETURN") {
						// 
					}

					if (type == "C_CALL") {
						// 
					}
				}
			}

			assemblyFile.close();
		}

		string trimTabs(string text) {
			text.erase(remove(text.begin(), text.end(), '\t'), text.end());
			return text;
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

		string format(string content, unordered_map<string, string> replaceStrings) {
			for (auto &str : replaceStrings) {
				while (content.find(str.first) != string::npos) {
					content.replace(content.find(str.first), str.first.length(), str.second);
				}
			}
			return trimTabs(content);
		}

		void writeArithmetic() {
			translatedFile << format(arithmeticTable[command], {{"{id}", to_string(identifier)}}) << endl;
		}
		
		void writePushPop(string type) {
			string argument1 = arg1();
			string argument2 = to_string(arg2());
			string arg1Translated = argumentTable[argument1];
			string aOrM = (argument1 == "pointer" || argument1 == "constant" ? "A" : "M");
			string pushPopTemplate = 
				R"(@{arg1}
				D={aOrM})";

			if (argument1 == "constant") {
				arg1Translated = argument2;
			}

			if (argument1 == "static") {
				arg1Translated = name + "." + argument2;
			}
			
			// Add the second argument to the equation, if not already handled via creation of the first argument
			// (in other words, if its not static or constant, add arg2)
			if (argument1 != "static" && argument1 != "constant") {
				pushPopTemplate += type == "C_PUSH" 
					? R"(
						@{arg2}
						A=D+A
						D=M)"
					: R"(
						@{arg2}
						D=D+A)";
			}

			pushPopTemplate += type == "C_PUSH" 
				? R"(
					@SP
					M=M+1
					A=M-1
					M=D)"
				: R"(
					@translator_temp
					M=D
					@SP
					M=M-1
					A=M
					D=M
					@translator_temp
					A=M
					M=D)";
			
			translatedFile << 
				format(pushPopTemplate, {
					{ "{arg1}", arg1Translated },
					{ "{arg2}", argument2 },
					{ "{aOrM}", aOrM }
				}) << endl;
		}
};

int main(){
	int identifier = 0;
	string userInput;

	cin >> userInput;
	ofstream translatedFile(userInput + ".asm");
	
	// Temporary init of sp
	translatedFile << R"(@256
D=A
@SP
M=D
)";

	if (fs::is_directory(userInput)) {
		for (auto const& file : fs::directory_iterator(userInput)) {
			auto path = file.path();
			
			if (path.extension() == ".vm") {
				Parser parser(translatedFile, path.stem().string(), identifier);
			}
		}
	} else {
		Parser parser(translatedFile, userInput, identifier);
	}

	translatedFile.close();

	cout << "Your .asm file is now ready. Enjoy.";

	return 0;
}
