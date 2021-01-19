#include "WinReg.h"

WinReg::WinReg(QSqlDatabase *bd, QSqlQuery *querydb, QWidget *wl) : layout(new QBoxLayout(QBoxLayout::Down)), butreg(new QPushButton("Sign in")), butback(new QPushButton("Log in")),
linelogin(new QLineEdit()), linepassword(new QLineEdit()), db(bd), query(querydb), winLogin(wl)
{
	setWindowTitle("Sign in");
	setLayout(layout);
	setFixedSize(200, 150);

	linelogin->setPlaceholderText("Login");
	linepassword->setPlaceholderText("Password");

	linepassword->setEchoMode(QLineEdit::EchoMode::Password);

	layout->addWidget(linelogin);
	layout->addWidget(linepassword);
	layout->addWidget(butreg);
	layout->addWidget(butback);

	QObject::connect(butreg, SIGNAL(clicked()), this, SLOT(clickButReg()));
	QObject::connect(butback, SIGNAL(clicked()), this, SLOT(clickButBack()));
}

WinReg::~WinReg()
{
	delete linelogin;
	delete linepassword;

	delete butreg;
	delete butback;

	delete layout;
}

void WinReg::clickButReg()
{
	if(query->exec("insert into accounts values(null, '" + linelogin->text() + "', '" + QCryptographicHash::hash(linepassword->text().toUtf8(), QCryptographicHash::Sha256).toHex() + "')"))
	{
		QMessageBox::warning(this, "Debug", "Success");
	}
	else
	{
		QMessageBox::warning(this, "Debug", query->lastError().text());
	}
}

void WinReg::clickButBack()
{
	winLogin->show();

	close();
}
