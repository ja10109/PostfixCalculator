/*
Jason Cho
CSCI-235
Professor Ligorio
Project 7: Postfix Calculator
PostfixCalculator.cpp
implementation file for PostfixCalculator
*/

#include "PostfixCalculator.hpp"
#include "PrecondViolatedExcep.hpp"

PostfixCalculator::PostfixCalculator(){}
//default constructor

/**
 @post Converts the string into a proper postfix expression.
 @param infix_expression
 @return the postfix expression or PrecondViolatedExcep if the infix_expression is invalid
 */
std::string PostfixCalculator::convertToPostfix(std::string infix_expression)
{
	//remove all spaces
	removeSpaces(infix_expression);
	//check for invalid operators in infix_expression 
	bool check = true;
	for(int i = 0;i < infix_expression.length();i++)
	{
		char c = infix_expression[i];
		if(operatorCheck(c) || isdigit(c) || c == '(' || c == ')' || c == ' ')
		{
			check = false;
		}
	}
	//PrecondViolatedExcep messages first reads the infix_expression and
	//checks whether infix_expression can be converted into a postfix
	if(!parenthesis(infix_expression))
		throw(PrecondViolatedExcep("Unbalanced parenthesis"));
	else if(check)
		throw(PrecondViolatedExcep("Unknown operator"));
	
	//infix_expression can be then converted into postfix_result
	else
	{
		for(int i = 0;i < infix_expression.length();i++)
		{
			char c = infix_expression[i];
		    	//Case: integers
			if(isdigit(c))
			{
				postfix_result += c;
				while(isdigit(infix_expression[++i]))
				{
					postfix_result += c;//if number has more than one integer 
				}
				postfix_result += ' ';
			}
			//Case: open parenthesis
			if(c == '(')
			{
				infix_stack.push(c);
			}
			//Case: operator
			if(operatorCheck(c))
			{
				//if the operator has a higher prescendece append the top of the infix stack to the postfix
				while(!infix_stack.empty() && infix_stack.top() != '(' 
				&& prescedence(infix_stack.top(),c))
				{
					postfix_result += infix_stack.top();
					postfix_result += ' ';
					infix_stack.pop();
				}
				infix_stack.push(infix_expression[i]);
			}
			//Case: closed parenthesis
			if(c == ')')
			{
				while(infix_stack.top() != '(' && !infix_stack.empty())
				{
					postfix_result += infix_stack.top(); //top is an operator 
					postfix_result += ' ';
					infix_stack.pop(); //pop the operator 
				}
				//removes the opening parnthesis
				infix_stack.pop();
			}
	    }
	    //append the rest of the infix stack onto the postfix
	    while(!infix_stack.empty())
	    {
	    	postfix_result += infix_stack.top();
	    	postfix_result += ' ';
		infix_stack.pop();
	    }
	}
	return postfix_result;
}

/**
 @post Takes the postfix expression and converts it into a double
 @param postfix_expression
 @return the result after calculating the postfix_expression
 */
double PostfixCalculator::calculatePostfix(std::string postfix_expression)
{
	std::string proxy = "";
	double op1,op2; //two operands used to preform either +,-,/,or *
	//convert the numbers in postfix into double
	for(int i = 0;i < postfix_expression.length();i++)
	{
		result = 0.0;
		if(isdigit(postfix_expression[i]))
		{
			proxy += postfix_expression[i];
		}
		if(i > 0)
		{
			if(postfix_expression[i] == ' ' && isdigit(postfix_expression[i-1]))
			{
				//converts all the numbers found in the string into double
				calculator_stack.push(atoi(proxy.c_str()));
				proxy = "";
			}
		}
		//case multiplication
		if(postfix_expression[i] == '*')
		{
			op2 = calculator_stack.top();
			calculator_stack.pop();
			op1 = calculator_stack.top();
			calculator_stack.pop();
			result = op1 * op2;
			calculator_stack.push(result);
		}
		//case division
		if(postfix_expression[i] == '/')
		{
			op2 = calculator_stack.top();
			calculator_stack.pop();
			op1 = calculator_stack.top();
			calculator_stack.pop();
			result = op1 / op2;
			calculator_stack.push(result);
		}
		//case additions
		if(postfix_expression[i] == '+')
		{
			op2 = calculator_stack.top();
			calculator_stack.pop();
			op1 = calculator_stack.top();
			calculator_stack.pop();
			result = op1 + op2;
			calculator_stack.push(result);
		}
		//case subtraction
		if(postfix_expression[i] == '-')
		{
			op2 = calculator_stack.top();
			calculator_stack.pop();
			op1 = calculator_stack.top();
			calculator_stack.pop();
			result = op1 - op2;
			calculator_stack.push(result);
		}
		//op1 and op2 are the first two numbers on the stack, that becomes calculated
		//into what the operand tells them to which then gets added on to the stack
	}
	return calculator_stack.top();
}

/**
 @post testCalculator takes an infix txt file and converts it into a 
 postfix txt file and calculated result file
 @param input_file_name takes "infix.txt"
 @return postfix.txt and results.txt with the correct postfix and calcuations
 or the PrecondViolatedExcep messages
 */
void PostfixCalculator::testCalculator(std::string input_file_name)
{
	std::ifstream infile(input_file_name);
	if(!infile)
	{
		exit(EXIT_FAILURE);
	}
	std::ofstream outfile1;
	outfile1.open("postfix.txt");
	std::ofstream outfile2;
	outfile2.open("results.txt");
	while(getline(infile,input_expression))
	//reads the lines of infix expressions
	{
		try //converts into postfix
		{
			input_expression = convertToPostfix(input_expression);
			outfile1 << input_expression << std::endl;
			outfile2 << calculatePostfix(input_expression) << std::endl;
		}
		catch(PrecondViolatedExcep fail)
		{
			outfile1 << fail.what() << std::endl;
			outfile2 << fail.what() << std::endl;
		}
	}
	infile.close();
	outfile1.close();
	outfile2.close();
}

/**
 @post parenthesis checks whether the parantheis in the 
 infix string is balanced
 @param infix_expression is the string being read
 @return true or false whether the paranthesis is balanced
 */
bool PostfixCalculator::parenthesis(std::string infix_expression)
{
	int parenthesis_count = 0;
	for(int i = 0; i < infix_expression.length(); i++)
	{
		//parenthesis_count increments if open paranthesis
		if(infix_expression[i] == '(')
		{
			parenthesis_count++;
		}
		//parenthesis_count decrements if closed parenthesis
		else if(infix_expression[i] == ')')
		{
			parenthesis_count--;
		}
	}
	//means there are more open parantheis than closed
	//or more closed than opened
	if(parenthesis_count != 0)
	{
		return false;
	}
	return true;
}

/**
 @post operatorCheck checks if infix string doesn't have
 an invalid operator
 @param infix_expression is the string being read
 @return true or false if the infix doesn't have
 an ivalid operator
 */
bool PostfixCalculator::operatorCheck(char c)
{
    	//check for unknown operator
   	if(c == '*' || c == '/' || c == '+' || c == '-')
   	{
		//if character in infix_expression is legal, it would return true
		return true;
	}
	return false;
}

/**
@post getPrescedence which checkns operator and sets a value depending on the operator
@param reads the string for an operator such as '*' '/' '+' '-'
@return values for operators
*/
int PostfixCalculator::getPrescedence(char op)
{
	if(op == '+' || op == '-')
	{
		return 1;
	}
	if(op == '*' || op == '/') //higher presecendence
	{
		return 2;
	}
	return 0;
}

/**
 @post prescedence checks which operator has a higher prescedence
 @param char op1, op2 which can be *,-,+, or /
 @return true if p1 == p2 or p1 > p2
 */
bool PostfixCalculator::prescedence(char op1, char op2)
{
	int p1 = getPrescedence(op1);
	int p2 = getPrescedence(op2);
	if(p1 == p2)
	{
		return true;
		//left to right in the expression, left comes first so left has higher prescedence
	}
	return(p1 > p2 ? true : false);
	//true if p1 is greater otherwise false 
}

/**
@post removeSpaces removes all spaces in the infix expression
@param infix_expression string being shortened
@return infix without any spaces
*/
void PostfixCalculator::removeSpaces(std::string infix_expression)
{
	for(int i = 0;i < infix_expression.length();i++)
	{
		if(isspace(infix_expression[i]))
		{
			infix_expression.erase(infix_expression.begin() + i);
			i--;
		}
	}
}
