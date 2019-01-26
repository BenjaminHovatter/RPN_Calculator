#include "ui_RPN_Calculator.h"
#include "RPN_Calculator.h"
#include "Calculator.h"
#include <string>

std::string equation;
std::string prevAnswer = "0";
bool calcFinished = true;

RPN_Calculator::RPN_Calculator(QWidget *parent)
	: QMainWindow(parent)
	
{
	ui.setupUi(this);
	setFixedSize(size());

	//ui.Display->setText("");

	//assign signals to buttons and connect them to the buttonPress() slot
	//Assign number buttons
	for (int i = 0; i < 10; i++) {
		QString buttonName = "pushButton_" + QString::number(i);
		connect(RPN_Calculator::findChild<QPushButton*>(buttonName), SIGNAL(clicked()), this, SLOT(buttonPress()));
	}
	//connect other operators
	connect(ui.pushButton_Add, SIGNAL(clicked()), this, SLOT(buttonPress()));
	connect(ui.pushButton_Minus, SIGNAL(clicked()), this, SLOT(buttonPress()));
	connect(ui.pushButton_Multiply, SIGNAL(clicked()), this, SLOT(buttonPress()));
	connect(ui.pushButton_Divide, SIGNAL(clicked()), this, SLOT(buttonPress()));
	connect(ui.pushButton_Modulus, SIGNAL(clicked()), this, SLOT(buttonPress()));
	connect(ui.pushButton_Exp, SIGNAL(clicked()), this, SLOT(buttonPress()));
	connect(ui.pushButton_OpenParenth, SIGNAL(clicked()), this, SLOT(buttonPress()));
	connect(ui.pushButton_CloseParenth, SIGNAL(clicked()), this, SLOT(buttonPress()));
	connect(ui.pushButton_Decimal, SIGNAL(clicked()), this, SLOT(buttonPress()));

	//connect backspace and clear
	connect(ui.pushButton_Back, SIGNAL(clicked()), this, SLOT(backspace()));
	connect(ui.pushButton_Clear, SIGNAL(clicked()), this, SLOT(clearDisp()));

	//Connect equals button
	connect(ui.pushButton_Equal, SIGNAL(clicked()), this, SLOT(calculate()));



}

void RPN_Calculator::buttonPress() {
	QPushButton *button = qobject_cast<QPushButton*>(sender()); //assign pointer 'button' to a pointer to the sender QObject (cast as a QPushButton)
	
	if (button->text() == "Mod") {
		equation += "%";
	}
	else if((isdigit(((button->text()).toStdString())[0]) || button->text() == ".") && calcFinished == true){
		//reset equation if new calculation
		equation = (button->text()).toStdString();
	}
	else {
		//else, button is an operator. Add to equation string.
		equation += (button->text()).toStdString();
	}
	//update display
	ui.Display->setText(QString::fromStdString(equation));
	calcFinished = false;
}

void RPN_Calculator::clearDisp() {
	//clear display
	equation = "";
	ui.Display->setText("");
}

void RPN_Calculator::backspace() {
	if (equation.length() > 0) {
		equation.pop_back();
	}
	ui.Display->setText(QString::fromStdString(equation));
}

void RPN_Calculator::calculate() {
	double answer = calculator(equation, prevAnswer);

	//prevAnswer is pointless/over-complex at the moment. Will create a solution at a later time.

	equation = std::to_string(answer);
	
	//remove any trailing zeroes (if any) from the type-converted calculation.
	while(equation.back() == '0') {
		equation.pop_back();
	}
	if (equation.back() == '.') {
		equation.pop_back();
	}

	//update display
	ui.prevAnswerBox->setText(QString::number(answer));
	ui.Display->setText(QString::number(answer));
	calcFinished = true;
}