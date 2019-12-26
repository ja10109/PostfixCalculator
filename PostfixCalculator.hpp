/*
Jason Cho
CSCI-235
Professor Ligorio
Project 7: Postfix Calculator
PostfixCalculator.hpp
header file for class PostfixCalculator
*/
#ifndef POSTFIXCALCULATOR_HPP_
#define POSTFIXCALCULATOR_HPP_

#include <string>
#include <iostream>
#include <fstream>
#include <stack>
#include <stdexcept>

class PostfixCalculator
{
	public:
		PostfixCalculator();
		//default constructor
		
		std::string convertToPostfix(std::string infix_expression);
		/**
 		@post Converts the string into a proper postfix expression.
 		@param infix_expression
 		@return the postfix expression or PrecondViolatedExcep if the infix_expression is invalid
 		*/
		
		double calculatePostfix(std::string postfix_expression);
		/**
 		@post Takes the postfix expression and converts it into a double
 		@param postfix_expression
 		@return the result after calculating the postfix_expression
 		*/
		
		void testCalculator(std::string input_file_name);
		/**
 		@post testCalculator takes an infix txt file and converts it into a 
 		postfix txt file and calculated result file
 		@param input_file_name takes "infix.txt"
 		@return postfix.txt and results.txt with the correct postfix and calcuations
 		or the PrecondViolatedExcep messages
 		*/
	
		bool parenthesis(std::string infix_expression);
		/**
 		@post parenthesis checks whether the parantheis in the 
 		infix string is balanced
 		@param infix_expression is the string being read
 		@return true or false whether the paranthesis is balanced
 		*/
		
		bool operatorCheck(char c);
		/**
 		@post operator_check checks if char is a valid operator
 		@param char c is the char being evaluted
 		@return true or false whether char c is an operator
 		*/
		
		int getPrescedence(char op);
		/**
		post get_prescedence which checkns operator and sets a value depending on the operator
		@param reads the string for an operator such as '*' '/' '+' '-'
		@return values for operators
		*/
			
		bool prescedence(char op1, char op2);
		/**
 		@post prescedence checks which operator has a higher prescedence
 		@param char op1, op2 which can be *,-,+, or /
 		@return true if p1 == p2 or p1 > p2
 		*/
	
		void removeSpaces(std::string infix_expression);
		/**
		@post removeSpaces removes all spaces in the infix expression
		@param infix_expression string being shortened
		@return infix without any spaces
		*/
	
	private:
		std::stack<char> infix_stack; //stack used for postfix conversion
		std::string postfix_result; //postfix expression
		std::stack<double> calculator_stack; //calculated postfix
		double result; //calculated result added on to the stack
		std::string input_expression; //string read from "input.txt" for test calculator
};
#endif
