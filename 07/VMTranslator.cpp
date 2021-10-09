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
		string fileName;
		string functionName;
		string command;
		int &identifier;
		ofstream &translatedFile;
		
		Parser(ofstream &_file, string _path, string _name, int &_id) : translatedFile(_file), functionName(_name), fileName(_name), identifier(_id) {
			string line;
			ifstream assemblyFile(_path);

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
						writeLabel();
					}

					if (type == "C_GOTO") {
						writeGoto();
					}

					if (type == "C_IF-GOTO") {
						writeIfGoto();
					}

					if (type == "C_FUNCTION") {
						writeFunction();
					}

					if (type == "C_RETURN") {
						writeReturn();
					}

					if (type == "C_CALL") {
						writeCall();
					}

					translatedFile << endl;
				}

				identifier++;
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

		string format(string content, unordered_map<string, string> replaceStrings = {}) {
			for (auto &str : replaceStrings) {
				while (content.find(str.first) != string::npos) {
					content.replace(content.find(str.first), str.first.length(), str.second);
				}
			}
			return trimTabs(content);
		}

		void writeLabel() {
			translatedFile << format("({funcName}${arg1})", {
				{ "{funcName}", functionName }, 
				{ "{arg1}", arg1() }
			});
		}

		void writeGoto() {
			translatedFile << format(R"(@{funcName}${arg1}
				0;JMP
			)", {
				{ "{funcName}", functionName }, 
				{ "{arg1}", arg1() }
			});
		}

		void writeIfGoto() {
			translatedFile << format(R"(@SP
				M=M-1
				A=M
				D=M
				@{funcName}${arg1}
				D;JNE)", {
					{ "{arg1}", arg1() },
					{ "{funcName}", functionName }
				});
		}

		void writeFunction() {
			string funcTemplate = R"(({arg1}))";
			functionName = arg1();

			for (int i = 0; i < arg2(); i++) {
					funcTemplate += R"(
						@SP
						M=M+1
						A=M-1
						M=0)";
			}

			translatedFile << format(funcTemplate, {{ "{arg1}", functionName }});
		}
		
		void writeArithmetic() {
			translatedFile << format(arithmeticTable[command], {{"{id}", to_string(identifier)}});
		}
		
		void writePushPop(string type) {
			string argument1 = arg1();
			string argument2 = to_string(arg2());
			string arg1Translated = argumentTable[argument1];
			string aOrM = 
				argument1 == "pointer" ||
				argument1 == "constant" ||
				(argument1 == "static" && type == "C_POP") ? "A" : "M";
			string pushPopTemplate = 
				R"(@{arg1}
				D={aOrM})";

			if (argument1 == "constant") {
				arg1Translated = argument2;
			}

			if (argument1 == "static") {
				arg1Translated = fileName + "." + argument2;
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

			translatedFile << format(pushPopTemplate, {
				{ "{arg1}", arg1Translated },
				{ "{arg2}", argument2 },
				{ "{aOrM}", aOrM }
			});
		}

		void writeReturn() {
			translatedFile << format(R"(@LCL
				D=M
				@FRAME
				M=D
				@5
				A=D-A
				D=M
				@RET
				M=D
				@SP
				M=M-1
				A=M
				D=M
				@ARG
				A=M
				M=D
				@ARG
				D=M+1
				@SP
				M=D
				@FRAME
				A=M-1
				D=M
				@THAT
				M=D
				@2
				D=A
				@FRAME
				A=M-D
				D=M
				@THIS
				M=D
				@3
				D=A
				@FRAME
				A=M-D
				D=M
				@ARG
				M=D
				@4
				D=A
				@FRAME
				A=M-D
				D=M
				@LCL
				M=D
				@RET
				A=M
				0;JMP)");
		}

		void writeCall() {
			translatedFile << format(R"(@{arg1}$return.{id}
				D=A
				@SP
				M=M+1
				A=M-1
				M=D
				@LCL
				D=M
				@SP
				M=M+1
				A=M-1
				M=D
				@ARG
				D=M
				@SP
				M=M+1
				A=M-1
				M=D
				@THIS
				D=M
				@SP
				M=M+1
				A=M-1
				M=D
				@THAT
				D=M
				@SP
				M=M+1
				A=M-1
				M=D
				@SP
				D=M
				@{arg2}
				D=D-A
				@5
				D=D-A
				@ARG
				M=D
				@SP
				D=M
				@LCL
				M=D
				@{arg1}
				0;JMP
				({arg1}$return.{id}))", {
					{ "{arg1}", arg1() },
					{ "{arg2}", to_string(arg2()) },
					{ "{id}", to_string(identifier) }
				});
		}
};

void writeBootstrap(ofstream &translatedFile) {
	translatedFile << R"(@256
		D=A
		@SP
		M=D
		@Sys.init$return.0
		D=A
		@SP
		M=M+1
		A=M-1
		M=D
		@LCL
		D=M
		@SP
		M=M+1
		A=M-1
		M=D
		@ARG
		D=M
		@SP
		M=M+1
		A=M-1
		M=D
		@THIS
		D=M
		@SP
		M=M+1
		A=M-1
		M=D
		@THAT
		D=M
		@SP
		M=M+1
		A=M-1
		M=D
		@SP
		D=M
		@5
		D=D-A
		@ARG
		M=D
		@SP
		D=M
		@LCL
		M=D
		@Sys.init
		0;JMP
		(Sys.init$return.0)
		)";
}

int main(){
	int identifier = 0;
	string userInput;

	cin >> userInput;
	ofstream translatedFile(userInput + ".asm");

	writeBootstrap(translatedFile);
		
	if (fs::is_directory(userInput)) {
		for (auto const& file : fs::directory_iterator(userInput)) {
			auto path = file.path();
			
			if (path.extension() == ".vm") {
				Parser parser(translatedFile, fs::absolute(path).string(), path.stem().string(), identifier);
			}
		}
	} else {
		Parser parser(translatedFile, userInput + ".vm", userInput, identifier);
	}

	translatedFile.close();

	cout << "Your .asm file is now ready. Enjoy.";

	return 0;
}
