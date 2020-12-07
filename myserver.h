#ifndef MYSERVER_H
#define MYSERVER_H

#include <QTcpServer>



class MyServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit MyServer(QObject *parent = nullptr);
    void startServer();


signals:


private slots:
    void myNewConnection();
//    void logError(QAbstractSocket::SocketError socketError);

protected:
    void incomingConnection(qintptr socketDescriptor);

private:
    QTcpServer *tcpserver;


};

#endif // MYSERVER_H
