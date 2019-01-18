#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <map>
#include <stdexcept>
#include <math.h>
#include <algorithm>

using namespace std;

double constructNumber(string equation, int &i);
double calculate(string number1, string number2, char operatr);

int main() {

	//answer from most recent calculation is stored here. Default value is zero.
	string prevAnswer = "0";

	cout << "Valid operations are + - * / () ^ %" << endl;
	while (true) {
		vector<string> outputQueue;
		vector<string> operatorStack;
		string equation;

		getline(cin, equation);

		//remove all spaces from equation, easier to read numbers/operators
		equation.erase(remove(equation.begin(), equation.end(), ' '), equation.end());
		equation.erase(remove(equation.begin(), equation.end(), '\t'), equation.end());

		//Lower number = lower priority
		//No subtraction. Only negative numbers. **Removing the subtraction operator from the list also allows for more elegant code when preparing to calculate**
		map<char, int> operatorPriority = {
			{'+', 1},
			{'/', 2},
			{'*', 2},
			{'%', 2},
			{'^', 3},
		};


		//is the user's computation meant to include an answer stored from a previous calculation?
		if ((operatorPriority.count(equation.at(0)) > 0 || equation.at(0) == '-') && prevAnswer != "0") {
			outputQueue.push_back(prevAnswer);
		}
		
		//iterate through input string (operations to compute). Set up outputQueue.
		for (int i = 0; i < equation.length(); i++) {

			bool isSubtraction = false;

			if (isdigit(equation[i]) || equation[i] == '.') {
				string number = to_string(constructNumber(equation, i));
				outputQueue.push_back(number);
				//if parentheses follows number, interpret as multiplication
				if (equation.at(fmin(i+1, equation.length()-1)) == '(') {
					operatorStack.push_back("*");
				}
			}
			

			if (operatorPriority.count(equation.at(i)) > 0 || equation[i] == '-' || equation[i] == '(' || equation[i] == '[' || equation[i] == ')') {

				//is sign a negative/subtraction sign? 
				if (equation.at(i) == '-') {
					//if subtracting, change to plus and change following number to negative after working with operatorStack.
					if (isdigit(equation.at(fmax(i - 1, 0))) || equation.at(fmax(i - 1, 0)) == '.' || (i == 0 && outputQueue.size()>0)) {
						if (equation.at(i + 1) == '(') {
							equation.at(i+1) = '[';
							equation.at(i) = '+';
						}
						else {
							isSubtraction = true;
							equation.at(i) = '+';
						}
						
					}
					//if not subtraction, must indicate a negative number. Convert to negative.
					// '[' indicates that the resulting value after calculating parenthesis should be sign-flipped.
					else {
						if (equation.at(i + 1) == '(') {
							equation.at(i + 1) = '[';
							continue;
						}
						else {
							outputQueue.push_back(to_string(-(constructNumber(equation, ++i))));
							continue;
						}
					}
				}
		

					//determine operator precedence 
					//is stack empty?
				if (operatorStack.size() > 0) {
					map<char, int>::iterator currentVal = operatorPriority.find(equation.at(i));
					string sBack = operatorStack.back();
					char cBack = sBack[0];
					map<char, int>::iterator topOfStackVal = operatorPriority.find(cBack);
					//if current operator is a closing parenthesis, perform operations on all numbers up to closest "("

					//must check if equation.at(i) is a '(' or '[' before determining precedence, as '(' and '[' do not exist in operatorPriority map.
					switch (equation.at(i)) {
						case ')':
							while (operatorStack.back() != "(" && operatorStack.back() != "[") {
								//if a closing parenthesis is reached, empty stack
								outputQueue.push_back(operatorStack.back());
								operatorStack.pop_back();
							}

							//Is top of operatorStack an open bracket? If so, change the sign of the back of the outputQueue.
							if (operatorStack.back() == "[") {
								double negNumber = -(stod(outputQueue.back()));
								outputQueue.back() = to_string(negNumber);
							}
							//Delete peranthesis/bracket
							operatorStack.pop_back();
							break;
						//must check if equation.at(i) is a '(' or '[' before determining precedence, as '(' and '[' do not exist in operatorPriority map.
						case '(':
							operatorStack.push_back("(");
							break;
						case '[':
							operatorStack.push_back("[");
							break;
						default:
							if (operatorStack.back() != "(" && operatorStack.back() != "[" && topOfStackVal->second >= currentVal->second) {
								//pop top of stack to outputQueue. Push current operator to operatorStack.
								outputQueue.push_back(operatorStack.back());
								operatorStack.pop_back();
								string operatr = "";
								operatr += equation.at(i);
								operatorStack.push_back(operatr);
							}
							else {
								//if topOfStack value is less than current, push current operator to operatorStack
								string operatr = "";
								operatr += equation.at(i);
								operatorStack.push_back(operatr);
							}
						}	
				}
				else {
					if (equation.at(i) == ')') {
						cout << "Invalid operation" << endl;
						continue;
					}
					else {
						string operatr = "";
						operatr += equation.at(i);
						operatorStack.push_back(operatr);
					}
				}

				//If subtracting (adding a negative), push negative to output queue.
				if (isSubtraction == true) {
					outputQueue.push_back(to_string(-(constructNumber(equation, ++i))));
				}     
			}
		}

		//empty operator stack
		while(operatorStack.size() > 0){
			outputQueue.push_back(operatorStack.back());
			operatorStack.erase(operatorStack.end()-1);
		}

		//calculate outputQueue
		while (outputQueue.size() > 1) {
			int i = outputQueue.size() - 1;

			//search operator priority list 
			string strI1 = outputQueue.at(i - 1);
			char cI1 = strI1[0];
			string strI2 = outputQueue.at(i - 2);
			char cI2 = strI2[0];
		
			map<char, int>::iterator itr1 = operatorPriority.find(cI1);
			map<char, int>::iterator itr2 = operatorPriority.find(cI2);

			while (itr1 != operatorPriority.end() || itr2 != operatorPriority.end()) {
				if (itr1 != operatorPriority.end()) {
					i--;
					
					strI1 = outputQueue.at(i - 1);
					cI1 = strI1[0];
					strI2 = outputQueue.at(i - 2);
					cI2 = strI2[0];

					itr1 = operatorPriority.find(cI1);
					itr2 = operatorPriority.find(cI2);
				}
				else if (itr2 != operatorPriority.end()) {
					i -= 2;

					strI1 = outputQueue.at(i - 1);
					cI1 = strI1[0];
					strI2 = outputQueue.at(i - 2);
					cI2 = strI2[0];

					itr1 = operatorPriority.find(cI1);
					itr2 = operatorPriority.find(cI2);
				}
			}

			char cI = outputQueue.at(i)[0];
			double calculation = calculate(outputQueue.at(i - 2), outputQueue.at(i - 1), cI);
			outputQueue.at(i - 2) = to_string(calculation);
			outputQueue.erase(outputQueue.begin() + i);
			outputQueue.erase(outputQueue.begin() + i-1);
		}
	
		cout << endl << stod(outputQueue.at(0)) << endl;
		prevAnswer = outputQueue.at(0);
	}

	return 0;
}


//This iterates through the input string starting at a passed position (i) , and parses all numbers and any existing decimal point into a string (up until non-digits or non-decimal point). 
double constructNumber(string equation, int &i) {
	string stringNumber;

	//While we iterate through numbers in this function, increase i from iteraion in main() as well.
	while ((isdigit(equation.at(i)) || equation.at(i) == '.')) {
		stringNumber += equation.at(i);
		//if number ends in decimal without decimal places (14. vs 14.00), remove decimal.
		if (equation.at(i) == '.' && !isdigit(equation.at(fmin(i+1, equation.length()-1)))) {
			stringNumber.erase(stringNumber.back());
		}
		else if (i + 1 == equation.length()) {
			break;
		}
		else if (!isdigit(equation.at(fmin(i+1, equation.length()-1))) && equation.at(i+1) != '.') {
			break;
		}
		++i;
	}
	return stod(stringNumber);
}

//works without decrementing if digit is last in input string

double calculate(string number1, string number2, char operatr) {
	double calculation;
	switch (operatr) {
	case '+':
		calculation = stod(number1) + stod(number2);
		break;
	case '*':
		calculation = stod(number1) * stod(number2);
		break;
	case '/':
		calculation = stod(number1) / stod(number2);
		break;
	case '^':
		calculation = pow(stod(number1), stod(number2));
		break;
	case '%':
		calculation = fmod(stod(number1), stod(number2));
		break;
	default:
		cout << "Could not calculate " + number1 + " " << operatr << " " + number2 + ", invalid operator."<< endl;
		calculation = 0;
		break;
	}

	return calculation;
}