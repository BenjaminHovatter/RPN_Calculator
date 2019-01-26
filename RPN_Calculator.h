#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_RPN_Calculator.h"

class RPN_Calculator : public QMainWindow
{
	Q_OBJECT

public:
	RPN_Calculator(QWidget *parent = Q_NULLPTR);

private:
	Ui::RPN_CalculatorClass ui;
	
private slots:
	void buttonPress(); //Numbers, Operators, and deciaml point.
	void clearDisp(); //Clear view
	void backspace(); //Backspace.
	void calculate(); //Calculate. Call Calculator.cpp and pass operation
};
