#include <QApplication>

#include "WinServerLogin.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	WinServerLogin win;

	win.show();

	return app.exec();
}
