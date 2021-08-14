#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
using namespace std;

class Parser {
	public:
		vector<string> commands;
		int currentCommandIndex = -1;

		Parser(string fileName) {
			string line;
			ifstream fileContents;
			
			fileContents.open(fileName);
			
			while(getline(fileContents, line)) {
				line.erase(remove(line.begin(), line.end(), ' '), line.end());

				// If theres anything in the line, and its not a comment
				if (line != "" && line.substr(0, 2) != "//") {
					commands.push_back(line);
				}
			}
		}

		bool hasMoreCommands() {
			return currentCommandIndex < (int)commands.size();
		}

		void advance() {
			currentCommandIndex++;
		}

		string commandType() {
			char firstLetter = commands[currentCommandIndex][0];

			if (firstLetter == '@') {
				return "A_COMMAND";
			}

			if (firstLetter == '(') {
				return "L_COMMAND";
			}

			return "C_COMMAND";
		}

		string symbol() {
			string currentCommand = commands[currentCommandIndex];
			bool isLCommand = currentCommand[currentCommand.size() - 1] == ')';

			return currentCommand.substr(1, - (isLCommand ? 2 : 1));
		}

		string mnemonic(string type) {
			string currentCommand = commands[currentCommandIndex];
			int equalsDelimiter = currentCommand.find("=");
			int semicolonDelimiter;

			if (type == "dest") {
				return equalsDelimiter ? currentCommand.substr(0, equalsDelimiter) : "null";
			}

			semicolonDelimiter = currentCommand.find(";");

			if (type == "comp") {
				return currentCommand.substr(equalsDelimiter ? equalsDelimiter : (0, semicolonDelimiter));
			}

			if (type == "jump") {
				return semicolonDelimiter ? currentCommand.substr(semicolonDelimiter) : "null";
			}
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
				{ "M", "1100000" },
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
};

int main(){
	string fileName;
	cin >> fileName; 
	
	// Parser parsedFile(fileName);

	return 0;
}

