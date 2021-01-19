#ifndef WINRECMSG_H
#define WINRECMSG_H

#include <QObject>

#include <QWidget>
#include <QBoxLayout>
#include <QTextEdit>
#include <QPushButton>
#include <QLabel>

#include <QUdpSocket>
#include <QNetworkDatagram>
#include <QHostAddress>

#include <QList>

#include <QDebug>

class WinRecMsg : public QWidget
{
	Q_OBJECT

	public:
		WinRecMsg(QWidget*);
		~WinRecMsg();

	private slots:
		void clickButExit();
		void ready();

	private:
		QBoxLayout *layout;

		QLabel *labelhost;
		QLabel *labelport;

		QTextEdit *linemessages;

		QPushButton *butexit;

		QWidget *winLogin;

		QUdpSocket *server;
		QUdpSocket *socket;

		QList<QHostAddress> *listaddrs;
};

#endif
