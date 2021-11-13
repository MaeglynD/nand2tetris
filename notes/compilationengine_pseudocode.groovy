
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
	
	// paramaterList not included

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

someFunction(fx(a, b), fy(2, 4))

compileTerm() {
	currentTokenReplica = currentToken;
	advance();

	<term>
		// Could be identifier, keyword, string const, int const etc.
		<{ currentTokenReplica.type }>
			if (currentTokenReplica.type === 'identifier') {
				if (currentToken == ';') {
					// Do nothing
				} else if (currentToken == '[') {
					// Get whatever's inside the square brackets
					compileExpression(']')
				} else if (currentToken == '(' || '.') {
					getContentsUntilSymbol('(');
					compileExpression(')');
				}
				
				{ currentToken }
				advance();
			}
		</{ currentTokenReplica.type }>
	</term>
}
	
	let whatever = test;
	 = test.testagain();
	 = test[0]
	 = test(test(test()));
	

	// <expression>
		// if (tokenToDelimitate == ')') {
		// 	depth = 0;
		// 	failsafe = 0;
			
			// while (!(currentToken == ')' && depth == 0)) {
			// 	{ here you would write the tag, as getContentsUntilSymbol does}
				
			// 	if (currentToken == '(') {
			// 		depth++;
			// 	}

			// 	if (currentToken == ')') {
			// 		depth--;
			// 	}

			// 	if (failsafe > 1000) {
			// 		print "
			// 		 compileExpression was broken prematurely as it reached the failsafe limit
			// 		"
			// 		break;
			// 	}

			// 	failsafe++
			// }
		// } else {
		// 	getContentsUntilSymbol(tokenToDelimitate);
		// }
	// </expression>
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

