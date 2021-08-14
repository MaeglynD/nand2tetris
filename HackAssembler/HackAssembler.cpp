#include <iostream>
#include <fstream>
#include <string>
#include <vector>
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
			return currentCommandIndex < commands.size();
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

int main(){
	string fileName;
	cin >> fileName; 
	
	Parser parsedFile(fileName);

	return 0;
}

