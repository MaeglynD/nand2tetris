
unordered_map<string, string> keywords = {
	...keywords,
};

symbols = "{}()[].,;+-*/&|<>=-"
currentToken = "";
multiLineCommentActive = false;

// We go line by line to account for single line comments
while (line in file) {
	while (characters in line) {

		if (whitespace) {
			// skip character
			continue;
		}

		if (multiLineCommentActive) {
			if (starts with '*/') {
				remove '*/'
				multiLineCommentActive = false;
			}

			continue;
		}

		if (starts with '//') {
			break; // go to next line
		}

		if (starts with '/*') {
			remove '/*'
			multiLineCommentActive = true;
			continue;
		}

		if (first characters a number) {
			currentToken = get the entire number
		}

		if (first characters a ' " ') {
			currentToken = get everything between the quotations - ""
		}

		// if its a symbol
		if (symbols.find_first_of(first character) != string::npos) {
			currentToken = first character
		}

		for (entry in keywords) {
			// if entry.key == line.substr(0, entry.key.length)
			if (entry.key is present from beginning) {
				currentToken = entry;
				break;
			}
		}

		// Identifier
		symbolPos = line.find_first_of(symbols);

		if (symbolPos != string::npos) {
			currentToken = line.substr(0, symbolPos);
		}
		
		// If we still don't have a currentToken by this point theres a syntax error
	}
}
