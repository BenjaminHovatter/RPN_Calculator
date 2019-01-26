#include "RPN_Calculator.h"
#include "Calculator.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	RPN_Calculator w;
	w.show();
	return a.exec();
}
