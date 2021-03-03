#include "WinReg.h"

WinReg::WinReg(QSqlDatabase *bd, QSqlQuery *querydb, QWidget *wl) : layout(new QBoxLayout(QBoxLayout::Down)), butreg(new QPushButton("Зарегистрироваться")), butback(new QPushButton("Войти")),
linelogin(new QLineEdit()), linepassword(new QLineEdit()), db(bd), query(querydb), winLogin(wl)
{
	setWindowTitle("Регистрация");
	setLayout(layout);
	setFixedSize(200, 150);

	linelogin->setPlaceholderText("Логин");
	linepassword->setPlaceholderText("Пароль");

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
	query->prepare("insert into accounts values(null, :Login, :Password)");

	query->bindValue(":Login", linelogin->text());
	query->bindValue(":Password", QCryptographicHash::hash(linepassword->text().toUtf8(), QCryptographicHash::Sha256).toHex());

	if(query->exec())
	{
		QMessageBox::warning(this, "Сообщение", "Успех");
	}
	else
	{
		QMessageBox::warning(this, "Ошибка", query->lastError().text());
	}
}

void WinReg::clickButBack()
{
	winLogin->show();

	close();
}
