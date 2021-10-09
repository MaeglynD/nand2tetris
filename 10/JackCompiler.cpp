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

class CompilitationEngine {

	CompilitationEngine() {
		// 
	}

	void compileClass() {
		// 
	}

	void compileClassVarDec() {
		// 
	}

	void compileSubroutine(){ 

	}

	void compileParameterList() {
		// 
	}

	void compileVarDev() {
		// 
	}

	void compileStatements() {
		// 
	}

	void compileDo() {
		// 
	}

	void compileLet(){
		// 
	}

	void compileWhile() {
		// 
	}

	void compileReturn() {
		// 
	}

	void compileIf() {
		// 
	}

	void compileExpression() {
		// 
	}

	void compileTerm() {
		// 
	}

	void compileExpressionList() {
		// 
	}
};

class JackTokenizer {

	JackTokenizer() {
		// 
	}

	bool hasMoreTokens() {
		return false;
	}
	
	void advance() {
		// 
	}

	string tokenType() {
		return "";
	}

	string keyWord() {
		return "";
	}

	char symbol() {
		return 'A';
	}

	string identifier() {
		return "";
	}

	int intVal() {
		return 0;
	}

	string stringVal() {
		return "";
	}

};

class JackAnalyzer {
	JackAnalyzer(string userInput) {
			if (fs::is_directory(userInput)) {
				for (auto const& file : fs::directory_iterator(userInput)) {
					auto path = file.path();
					
					if (path.extension() == ".jack") {
						analyze("./" + userInput + "/" + path.stem().string());
					}
				}
		} else {
			// analuzye
			analyze(userInput);
		}
	}

	void analyze(string name) {
		ofstream xmlFile(name + ".xml");
		ifstream jackFile(name + ".jack");
	}
};


int main(){
	// string userInput;
	// cin >> userInput;

	// if (fs::is_directory(userInput)) {
	// 	for (auto const& file : fs::directory_iterator(userInput)) {
	// 		auto path = file.path();
			
	// 		if (path.extension() == ".jack") {
	// 			cout << fs::absolute(path).string() << endl;
	// 		}
	// 	}
	// }

	return 0;
}
