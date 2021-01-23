#include "WinMsg.h"

WinMsg::WinMsg(QWidget *wl, QString name) : layout(new QBoxLayout(QBoxLayout::Down)), butsend(new QPushButton("Send")), butexit(new QPushButton("Exit")),
labellogin(new QLabel("Ваш ник: " + name)), labelhost(new QLabel("Ваш адрес: ")), labelport(new QLabel("Ваш порт: ")),
linemsg(new QLineEdit()), linehost(new QLineEdit()), linechat(new QTextEdit()),
winLogin(wl), socket(new QUdpSocket(this)), server(new QUdpSocket(this)), login(new QString(name))
{
	setWindowTitle("Чат");
	setLayout(layout);
	setFixedSize(500, 500);

	socket->bind(5554, QUdpSocket::ShareAddress);

	labelhost->setText(labelhost->text() + socket->localAddress().toString());
	labelport->setText(labelport->text() + QString::number(5554));

	linechat->setReadOnly(true);
	linechat->setPlaceholderText("Сообщения");

	linehost->setPlaceholderText("Адрес");
	linemsg->setPlaceholderText("Текст сообщения...");

	layout->addWidget(labellogin);
	layout->addWidget(linechat);
	layout->addWidget(linemsg);
	layout->addWidget(butsend);
	layout->addWidget(butexit);

	QObject::connect(butsend, SIGNAL(clicked()), this, SLOT(clickButSend()));
	QObject::connect(butexit, SIGNAL(clicked()), this, SLOT(clickButExit()));
	QObject::connect(socket, SIGNAL(readyRead()), this, SLOT(recive()));
}

WinMsg::~WinMsg()
{

	if(socket)
	{
		socket->close();

		delete socket;

		socket = nullptr;
	}

	if(server)
	{
		server->close();

		delete server;

		server = nullptr;
	}

	delete login;

	delete butsend;
	delete butexit;

	delete labellogin;
	delete labelhost;

	delete linemsg;

	delete linehost;

	delete linechat;

	delete layout;
}

void WinMsg::clickButSend(){ server->writeDatagram(("[" + *login + "] : " + linemsg->text()).toUtf8(), QHostAddress::Broadcast, 5555); }

void WinMsg::clickButExit()
{
	if(socket)
	{
		socket->close();

		 delete socket;

		socket = nullptr;
	}

	if(server)
	{
		server->close();

		delete server;

		server = nullptr;
	}

	winLogin->show();

	close();
}

void WinMsg::recive()
{
	QNetworkDatagram datagram = socket->receiveDatagram();
	QString msg = "";

	msg = QString(datagram.data());

	linechat->setText(linechat->toPlainText() + msg);
}
