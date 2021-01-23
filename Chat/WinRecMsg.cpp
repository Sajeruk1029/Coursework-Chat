#include "WinRecMsg.h"

WinRecMsg::WinRecMsg(QWidget *wl) : layout(new QBoxLayout(QBoxLayout::Down)), labelhost(new QLabel("Ваш адрес: ")), labelport(new QLabel("Ваш порт: ")),
linemessages(new QTextEdit()), butexit(new QPushButton("Выход")), winLogin(wl),
server(new QUdpSocket(this)), socket(new QUdpSocket(this))
{
	setWindowTitle("Сервер");
	setFixedSize(500, 500);
	setLayout(layout);

	linemessages->setPlaceholderText("Сообщения");
	linemessages->setReadOnly(true);

	socket->bind(5555, QUdpSocket::ShareAddress);

	labelhost->setText(labelhost->text() + server->localAddress().toString());
	labelport->setText(labelport->text() + QString::number(5555));

	layout->addWidget(linemessages);
	layout->addWidget(butexit);

	QObject::connect(butexit, SIGNAL(clicked()), this, SLOT(clickButExit()));
	QObject::connect(socket, SIGNAL(readyRead()), this, SLOT(ready()));
}

WinRecMsg::~WinRecMsg()
{
	if(server)
	{
		server->close();

		delete server;

		server = nullptr;
	}

	if(socket)
	{
		socket->close();

		delete socket;

		socket = nullptr;
	}

	delete labelhost;
	delete labelport;

	delete linemessages;

	delete butexit;

	delete layout;
}

void WinRecMsg::clickButExit()
{
	if(server)
	{
		server->close();

		delete server;

		server = nullptr;
	}

	if(socket)
	{
		socket->close();

		delete socket;

		socket = nullptr;
	}

	winLogin->show();

	close();
}

void WinRecMsg::ready()
{
	QNetworkDatagram datagram = socket->receiveDatagram();
	QString msg = "";

	msg =  QString(datagram.senderAddress().toString().split(':')[3]) + ":" + QString::number(datagram.senderPort()) + QString(datagram.data() + "\n");

	linemessages->setText(linemessages->toPlainText() + msg);

	server->writeDatagram(msg.toUtf8(), QHostAddress::Broadcast, 5554);
}
