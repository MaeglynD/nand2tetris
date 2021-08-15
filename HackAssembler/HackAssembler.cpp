#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
using namespace std;

class SymbolTable{
	public: 
		SymbolTable() {
			for (int i = 0; i <= 15; i++){
				table["R" + i] = i;
			}
		}

		unordered_map<string, int> table{
			{ "SP", 0 },
			{ "LCL", 1 },
			{ "ARG", 2 },
			{ "THIS", 3 },
			{ "THAT", 4 },
			{ "SCREEN", 16384 },
			{ "KBD", 24576 },
		};

		void addEntry(string symbol, int address) {
			table[symbol] = address;
		}

		bool contains(string symbol) {
			return table.find(symbol) != table.end();
		}

		int getAddress(string symbol) {
			return table[symbol];
		}
};

class Code{
	public:
		string dest(string mnemonic) {
			unordered_map<string, string> comparisons({
				{ "null", "000" },
				{ "M", "001" },
				{ "D", "010" },
				{ "MD", "011" },
				{ "A", "100" },
				{ "AM", "101" },
				{ "AD", "110" },
				{ "AMD", "111" },
			});

			return comparisons[mnemonic];
		}

		string comp(string mnemonic) {
			unordered_map<string, string> comparisons({
				{ "0", "0101010" },
				{ "1", "0111111" },
				{ "-1", "0111010" },
				{ "D", "0001100" },
				{ "A", "0110000" },
				{ "!D", "0001101" },
				{ "!A", "0110001" },
				{ "-D", "0001111" },
				{ "-A", "0110011" },
				{ "D+1", "0111111" },
				{ "A+1", "0110111" },
				{ "D-1", "0001110" },
				{ "A-1", "0110010" },
				{ "D+A", "0000010" },
				{ "D-A", "0010011" },
				{ "A-D", "0000111" },
				{ "D&A", "0000000" },
				{ "D|A", "0010101" },
				{ "M", "1110000" },
				{ "!M", "1110001" },
				{ "-M", "1110011" },
				{ "M+1", "1110111" },
				{ "M-1", "1110010" },
				{ "D+M", "1000010" },
				{ "D-M", "1010011" },
				{ "M-D", "1000111" },
				{ "D&M", "1000000" },
				{ "D|M", "1010101" },
			});

			return comparisons[mnemonic];
		}

		string jump(string mnemonic) {
			unordered_map<string, string> comparisons({
				{ "null", "000" },
				{ "JGT", "001" },
				{ "JEQ", "010" },
				{ "JGE", "011" },
				{ "JLT", "100" },
				{ "JNE", "101" },
				{ "JLE", "110" },
				{ "JMP", "111" },
			});

			return comparisons[mnemonic];
		}

		string intToBinary(int num) {
			string binaryString = "0000000000000000";

			while(num > 0) {
				if (num == 1) {
					binaryString[15] = '1';
					break;
				} 

				int i = 1;

				while ((1 << i) <= num) i++;
				num -= (1 << (i - 1));
				binaryString[16 - i] = '1';
			}

			return binaryString;
		}
};
class Parser {
	public:
		vector<string> commands;
		SymbolTable symbolTable;

		Parser(string fileName) {
			int commandIndex = 0;
			string line;
			Code translator;
			ifstream assemblyFile(fileName + ".asm");
			ofstream translatedFile(fileName + ".hack");

			while(getline(assemblyFile, line)) {
				line.erase(remove(line.begin(), line.end(), ' '), line.end());

				if (line != "" && line.substr(0, 2) != "//") {
					commands.push_back(line);
					
					if (commandType(line) == "L_COMMAND") {
						symbolTable.addEntry(symbol(line), commandIndex);
					} else {
						commandIndex++;
					}
				}
			}

			for (string command : commands) {
				string binary;
				string currentType = commandType(command);

				if (currentType == "A_COMMAND") {
					binary = translator.intToBinary(stoi(symbol(command)));
				} 
				
				if (currentType == "C_COMMAND") {
					binary = "111" 
						+ translator.comp(DCJ(command, "comp")) 
						+ translator.dest(DCJ(command, "dest"))
						+ translator.jump(DCJ(command, "jump"));
				}
				
				if (currentType == "L_COMMAND") {
					//
				}

				translatedFile << binary << endl;
			}

			assemblyFile.close();
			translatedFile.close();
		}

		string commandType(string command) {
			char firstLetter = command[0];

			if (firstLetter == '@') {
				return "A_COMMAND";
			}

			if (firstLetter == '(') {
				return "L_COMMAND";
			}

			return "C_COMMAND";
		}

		string symbol(string command) {
			return command.substr(1, -(command[command.size() - 1] == ')' ? 2 : 1));
		}

		string DCJ(string command, string type) {
			int equalsDelimiter = command.find("=");
			int semicolonDelimiter = command.find(";");

			bool isSemiDelim = semicolonDelimiter != -1;
			bool isEqualDelim = equalsDelimiter != -1;

			if (type == "dest" && isEqualDelim) {
				return command.substr(0, equalsDelimiter);
			}

			if (type == "comp") {
				return isEqualDelim 
					? command.substr(equalsDelimiter + 1) 
					: command.substr(0, semicolonDelimiter);
			}

			if (type == "jump" && isSemiDelim) {
				return command.substr(semicolonDelimiter + 1);
			}

			return "null";
		}
};

int main(){
	string fileName;
	cin >> fileName; 
	
	Parser parser(fileName);
	cout << "Your .hack file is now ready. Enjoy.";

	return 0;
}

