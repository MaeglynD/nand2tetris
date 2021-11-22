
getContentsRecursively (bool stopAtEndBlock = false):
	currentToken = tokenizer.currentToken();
	tokenizer.advance();

	if (stopAtEndBlock && currentToken = '}') {
		break;
	}

	// 
	if ('field') {
		classVarDec();
	}

	else if ('method', 'function', 'constructor') {
		subroutine();
	}
	
	else if ('var') {
		varDec();
	}

	else if ('if', 'do', 'let', 'while', 'return') {
		compileStatements();
	}

	..

	if (tokenizer.hasMoreTokens()) {
		getContentsRecursively(stopAtEndBlock);
	}


classVarDec:
	<classVarDec>	
		{ getContentsUntilSymbol(';') }
	</classVarDec>


subroutine:
	<subroutine>
		{ getContentsUntilSymbol('(') }
		// Note could handle possible extra '(' in paramater list
		// for default functions i.e. f(string test = test())
		{ compileParamaterList() }
		{ getContentsUntilSymbol('{') }
		<subroutineBody>
			{ compileStatements() }
		</subroutineBody>
	</subroutine>


parameterList:
	<parameterList>
		{ getContentsUntilSymbol(')') }
	</parameterList>


varDec:
	<varDec>
		{ getContentsUntilSymbol(';') }
	</varDec>

// Invoked first then compileStatement invoked
compileStatements:
	if (currentToken == '}') {
		<symbol> } </symbol>
		advance();
	} else {
		if (current token is not 'if', ...) {
			throw error('compileStatements was invoked when currentToken is not a statement')
		}

		<statements>
			{
				while (currentToken is 'if', 'do', 'let', 'while', 'return') {
					compileStatement(currentToken)
				}
			}
		</statements>
	}

gcusAndRecurExpression(stoppingToken) {
	while (currentToken != stoppingToken) {
		if (currentToken is "(" or "[") {
			compileExpression(")" or "]");
		} else {
			wrapAndAdvanceCurrentToken();
		}
	}
}
	
compileStatement(string currentToken) {
	<{currentToken}Statement>
		{
			if (currentToken == 'if' || 'while') {
				getContentsUntilSymbol('(');
				compileExpression(')');
				getContentsUntilSymbol('{');
				compileStatements();
			}

			if (currentToken == 'let') {
				gcusAndRecurExpressions("=");
				compileExpression(';');
			}

			if (currentToken == 'do') {
				getContentsUntilSymbol('(');
				compileExpressionList();
			}

			if (currentToken == 'return') {
				<keyword> return </keyword>;
				advance();
				
				compileExpression(";");
			}


		}
	</{currentToken}Statement>
}

compileExpression(stoppingToken, shouldIncludeStopper) {
	// For the things like 'return;'
	if (currentToken == ';') {
		<symbol> ; </symbol>
		advance();
		return;
	}
	
	<expression>
		while (currentToken != stoppingToken) {
			compileTerm();
		}
	</expression>
}

compileTerm() {
	previousToken = currentToken;
	advance();

	<term>
		if (previousToken.type === 'identifier') {
			if (currentToken == '.') {
				getContentsUntilSymbol('(');
				compileExpressionList();
			}
			
			 else if (currentToken == '[') {
				compileExpression(']')
			}
			
			 else if (currentToken == '(') {
				compileExpression(')');
			} else {
				wrapAndAdvanceCurrentToken();
			}
		} else {
			wrapAndAdvanceCurrentToken();
		}
			
	</term>
}

let test = { term }
				 = 32 + someVariableName;
				 = someArrayName[32];
				 = someArrayName[someArrayNameTwo[2] + someVariableName];

testFunction({ term }, {})


compileExpressionList() {
	<expressionList>
		while (currentToken != ')') {
			if (currentToken == ',') {
				getContentsUntilSymbol(',');
			}
			compileExpression(',' || ')', false);
		}

		wrapAndAdvanceCurrentToken();
	</expressionList>
}

compileClass() {
	<class>
		 getContentsUntilSymbol('{');
		 getContentsRecursively(true);
	</class>
}

compileExpression:
	while (!isCurrentToken(stoppingToken)) {
		if (identifier || int_const || string_const || keyword || '(') {
			compileTerm()
		} else {
			wrapAndAdvance
		}
	}


compileTerm:
	string contents;
	prevToken = ...;

	wrapAndAdvance(contents);

	if (prevToken is identifier) {
		if (currentToken is '(') {
			contents += compileExpression({ ")" });
		}
		
		else if (currentToken is '[') {
			contents += compileExpression({ "]" });
		}

		else if (currentToken is '.') {
			contents += getContentsUntilSymbol("(")
				.append(compileExpressionList);
		}
	} 

	else if (prevToken is '-', '~') {
		wrapAndAdvanceCurrentToken(contents);
		contents += compileTerm()
	}

	return wrapInTags("term", contents);




let test = a[3] + test(3);
				 
= a[3] + a.test(3);
= 3 + test.someFunction();
<expression>
	<term>
		<identifier> a </identifier>
		<symbol> [ </symbol>
			<term>
				<int_const> 3 </int_const>
			</term>
	</term>
	<symbol> + </symbol>
	<term>
		<identifier> test </identifier>

	</term>
</expression>
<symbol> ; </symbol>

