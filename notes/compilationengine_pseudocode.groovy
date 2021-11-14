
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
			{ getContentsRecursively(true) }
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

	
compileStatement(string currentToken) {
	<{currentToken}Statement>
		{
			if (currentToken == 'if' || 'while') {
				getContentsUntilSymbol('(');
				compileExpression(true);
				getContentsUntilSymbol('{');
				compileStatements();
			}

			if (currentToken == 'let') {
				getContentsUntilSymbol('=');
				compileExpression();
				getContentsUntilSymbol(';');
			}

			if (currentToken == 'do') {
				getContentsUntilSymbol('(');
				compileExpressionList();
				getContentsUntilSymbol(';');
			}

			if (currentToken == 'return') {
				<keyword> return </keyword>;
				advance();
				
				compileExpression();
			}


		}
	</{currentToken}Statement>
}

compileExpression(stoppingToken) {
	if (currentToken == ';') {
		<symbol> ; </symbol>
		advance();
		return;
	}
	
	<expression>
			<term>

			while (currentToken != stoppingToken) {
				compileTerm();
			}

			</term>
		</expression>
}

compileTerm() {
	previousToken = currentToken;
	advance();

	<term>
		// Could be identifier, keyword, string const, int const etc.
		if (currentToken.type != 'symbol') {
			throw err 'We expected a symbol here.'
		}

		getContentsUntilSymbol(currentToken);

		if (previousToken.type === 'identifier') {
			if (currentToken == '.') {
				getContentsUntilSymbol('(');
				compileExpressionList(')');
			}

			if (currentToken == '[') {
				compileExpression(']')
			} else if (currentToken == '(') {
				compileExpressionList(')');
			}
			getContentsUntilSymbol(currentToken);
		}
			
	</term>
}

compileExpressionList() {
	<expressionList>
		while (currentToken != ')') {
			if (currentToken == ',') {
				getContentsUntilSymbol(',');
			}
			compileExpression(',');

		}
	</expressionList>
}

compileClass() {
	<class>
		 getContentsUntilSymbol('{');
		 getContentsRecursively(true);
	</class>
}