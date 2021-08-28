#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
using namespace std;

class Parser {
	public:
		vector<string> commands;

		Parser(string fileName) {
			string line;
			ifstream assemblyFile(fileName + ".vm");
			ofstream translatedFile(fileName + ".asm");

			while(getline(assemblyFile, line)) {
				line = line.substr(0, line.find("//"));
				line.erase(0, line.find_first_not_of("\t\n\v\f\r "));
				line.erase(line.find_last_not_of("\t\n\v\f\r ") + 1);

				if (line != "") {
					commands.push_back(line);
				}
			}

			assemblyFile.close();
			translatedFile.close();
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

			return "";
		}
};


int main(){
	string fileName;
	// cin >> fileName; 
	
	Parser parser(fileName);
	cout << "Your .asm file is now ready. Enjoy.";
	

	return 0;
}
