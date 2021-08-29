#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <experimental/filesystem>
using namespace std;
namespace fs = std::experimental::filesystem;

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
