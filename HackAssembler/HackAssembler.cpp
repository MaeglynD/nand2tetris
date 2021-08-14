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
			char firstLetter = commands.at(currentCommandIndex)[0];

			if (firstLetter == '@') {
				return "A_COMMAND";
			}

			if (firstLetter == '(') {
				return "L_COMMAND";
			}

			return "C_COMMAND";
		}

		string symbol() {
			string currentCommand = commands.at(currentCommandIndex);
			bool isLCommand = currentCommand[currentCommand.size() - 1] == ')';

			return currentCommand.substr(1, - (isLCommand ? 2 : 1));
		}
};

int main(){
	string fileName;
	cin >> fileName; 
	
	Parser parsedFile(fileName);

	return 0;
}

