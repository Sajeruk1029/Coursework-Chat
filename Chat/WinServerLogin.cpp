#include "WinServerLogin.h"

WinServerLogin::WinServerLogin() : layout(new QBoxLayout(QBoxLayout::Down)), linehostserver(new QLineEdit()), lineloginserver(new QLineEdit()),
linepasswordserver(new QLineEdit()), butconnect(new QPushButton("Connect server")), butdisconnect(new QPushButton("Disconnect server")),
butcontinue(new QPushButton("Continue")), butloginserver(new QPushButton("Log in as a server")), db(nullptr),
query(nullptr), winLogin(nullptr), winRecMsg(nullptr)
{
	setWindowTitle("Auth server");
	setLayout(layout);
	setFixedSize(300, 200);

	linehostserver->setPlaceholderText("Host");
	lineloginserver->setPlaceholderText("Login");
	linepasswordserver->setPlaceholderText("Password");

	linepasswordserver->setEchoMode(QLineEdit::EchoMode::Password);

	butdisconnect->setEnabled(false);
	butcontinue->setEnabled(false);

	layout->addWidget(linehostserver);
	layout->addWidget(lineloginserver);
	layout->addWidget(linepasswordserver);

	layout->addWidget(butconnect);
	layout->addWidget(butloginserver);
	layout->addWidget(butdisconnect);
	layout->addWidget(butcontinue);

	QObject::connect(butconnect, SIGNAL(clicked()), this, SLOT(clickButConnect()));
	QObject::connect(butdisconnect, SIGNAL(clicked()), this, SLOT(clickButDisconnect()));
	QObject::connect(butcontinue, SIGNAL(clicked()), this, SLOT(clickButContinue()));
	QObject::connect(butloginserver, SIGNAL(clicked()), this, SLOT(clickButServerLogin()));
}

WinServerLogin::~WinServerLogin()
{
	if(query){ delete query; }
	if(db){ delete db; }
	if(winLogin){ delete winLogin; }

	delete linehostserver;
	delete lineloginserver;
	delete linepasswordserver;

	delete butconnect;
	delete butdisconnect;
	delete butcontinue;

	delete layout;
}

void WinServerLogin::clickButConnect()
{
	db = new QSqlDatabase();

	*db = QSqlDatabase::addDatabase("QMYSQL", "conn");

	db->setHostName(linehostserver->text());
	db->setUserName(lineloginserver->text());
	db->setPassword(linepasswordserver->text());
	db->setDatabaseName("chat");

	if(db->open())
	{
		query = new QSqlQuery(*db);

		QMessageBox::warning(this, "Debug", "Success");

		butconnect->setEnabled(false);
		butdisconnect->setEnabled(true);
		butcontinue->setEnabled(true);

		linehostserver->setEnabled(false);
		lineloginserver->setEnabled(false);
		linepasswordserver->setEnabled(false);
	}
	else
	{
		QMessageBox::warning(this, "Debug", "Failed");

		QSqlDatabase::removeDatabase("conn");

		delete query;
		query = nullptr;

		delete db;
		db = nullptr;
	}
}

void WinServerLogin::clickButDisconnect()
{
	delete query;
	query = nullptr;

	db->close();

	QSqlDatabase::removeDatabase("conn");

	delete db;
	db = nullptr;

	butconnect->setEnabled(true);
	butdisconnect->setEnabled(false);
	butcontinue->setEnabled(false);

	linehostserver->setEnabled(true);
	lineloginserver->setEnabled(true);
	linepasswordserver->setEnabled(true);
}

void WinServerLogin::clickButContinue()
{
	winLogin = new WinLogin(db, query, this);

	winLogin->show();

	hide();
}

void WinServerLogin::clickButServerLogin()
{
	winRecMsg = new WinRecMsg(this);

	winRecMsg->show();

	close();
}
