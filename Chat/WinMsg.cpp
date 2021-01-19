#include "WinMsg.h"

WinMsg::WinMsg(QWidget *wl, QString name) : layout(new QBoxLayout(QBoxLayout::Down)), butsend(new QPushButton("Send")), butexit(new QPushButton("Exit")),
labellogin(new QLabel("Your login is: " + name)), labelhost(new QLabel("Your Address is: ")), labelport(new QLabel("Your port is: ")),
linemsg(new QLineEdit()), linehost(new QLineEdit()), linechat(new QTextEdit()),
winLogin(wl), socket(new QUdpSocket(this)), server(new QUdpSocket(this)), login(new QString(name))
{
	setWindowTitle("Chat");
	setLayout(layout);
	setFixedSize(500, 500);

	socket->bind(5554, QUdpSocket::ShareAddress);

	labelhost->setText(labelhost->text() + socket->localAddress().toString());
	labelport->setText(labelport->text() + QString::number(5554));

	linechat->setReadOnly(true);
	linechat->setPlaceholderText("Messages");

	linehost->setPlaceholderText("Address");
	linemsg->setPlaceholderText("Text message...");

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
