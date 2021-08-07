#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class Parser {
	ifstream fileContents;
	string currentCommand;

	public:
		Parser(string fileName){
			fileContents.open(fileName);
		}

		void advance() {
			getline(fileContents, currentCommand);
		}

		bool hasMoreCommands(){
			return fileContents.peek() != -1;
		}

		string commandType(){
			currentCommand.erase(remove(currentCommand.begin(), currentCommand.end(), ' '), currentCommand.end());
			
			if(currentCommand[0] == '@'){
				// A command
			}else{
				// 
			}
		}
};

int main(){
	string fileName;
	cin >> fileName; 
	
	Parser parsedFile(fileName);

	return 0;
}

