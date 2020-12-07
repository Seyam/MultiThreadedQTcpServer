#ifndef MYTHREAD_H
#define MYTHREAD_H

#define auth_token "FF0186000000000079"
#define data_token "FF0186000000000080"


#include <QTcpSocket>
#include <QThread>
#include <QDebug>

class MyThread : public QThread
{
    Q_OBJECT
public:
    explicit MyThread(qintptr ID,   QObject *parent = nullptr);
    void run();

signals:
    void error(QTcpSocket::SocketError socketerror);

    //these signals do not exist inside QTcpSocket, (those that exist need not to be declared here, they signal auto), so error thrown in console in runtime
//    void acceptError(QAbstractSocket::SocketError socketError);
//    void newConnection();

private:
    QTcpSocket *socket;
    int socketDescriptor;

public  slots:
    void readyRead();
    void disconnected();
    void errorThrown(QTcpSocket::SocketError socketerror);
//    void newConnection();
};

#endif // MYTHREAD_H
