#include "WinRecMsg.h"

WinRecMsg::WinRecMsg(QWidget *wl) : layout(new QBoxLayout(QBoxLayout::Down)), labelhost(new QLabel("Your Address is: ")), labelport(new QLabel("Your port is: ")),
linemessages(new QTextEdit()), butexit(new QPushButton("Exit")), winLogin(wl),
server(new QUdpSocket(this)), socket(new QUdpSocket(this)), listaddrs(new QList<QHostAddress>())
{
	setWindowTitle("Server");
	setFixedSize(500, 500);
	setLayout(layout);

	linemessages->setPlaceholderText("Messages");
	linemessages->setReadOnly(true);

	socket->bind(5555, QUdpSocket::ShareAddress);

	labelhost->setText(labelhost->text() + server->localAddress().toString());
	labelport->setText(labelport->text() + QString::number(5555));

	//layout->addWidget(labelhost);
	//layout->addWidget(labelport);
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

	delete listaddrs;

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

	delete listaddrs;

	winLogin->show();

	close();
}

void WinRecMsg::ready()
{
	QNetworkDatagram datagram = socket->receiveDatagram();
	QString msg = "";

	msg =  QString(datagram.senderAddress().toString().split(':')[3]) + ":" + QString::number(datagram.senderPort()) + QString(datagram.data() + "\n");

	linemessages->setText(linemessages->toPlainText() + msg);


	if((listaddrs->indexOf(datagram.senderAddress()) < 0))
	{
		listaddrs->append(datagram.senderAddress());
	}

	qDebug() << listaddrs->size();

	//for(unsigned char counter = 0; counter < listaddrs->size(); ++counter)
	//{
		//qDebug() << server->state();
		//server->connectToHost(listaddrs->value(counter), 5554);
		//qDebug() << server->state();
		//server->waitForConnected();
		//qDebug() << server->state();
		server->writeDatagram(msg.toUtf8(), QHostAddress::Broadcast, 5554);
		//qDebug() << server->state();
		//server->disconnectFromHost();
		//qDebug() << server->state();
	//}
}
