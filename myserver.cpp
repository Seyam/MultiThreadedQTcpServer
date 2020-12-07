#include "myserver.h"
#include "mythread.h"

MyServer::MyServer(QObject *parent) : QTcpServer(parent)
{
    qRegisterMetaType<QAbstractSocket::SocketError>("QAbstractSocket::SocketError");
//    tcpserver = new QTcpServer(this);//always give a new child object a parent
    //Connect signal and slot inside the constructor
    connect(this, SIGNAL(newConnection()),    this,   SLOT(myNewConnection()));
//    connect(this, SIGNAL(acceptError(QAbstractSocket::SocketError socketError)),    this,   SLOT(logError(QAbstractSocket::SocketError socketError)));
}


void MyServer::startServer()
{
    int port = 1234;

    if(!this->listen(QHostAddress::Any, port))
    {
        qDebug() << "Could not start the server";
    }
    else
    {
        qDebug() << "Listening at port: " << this->serverPort() << " at ip: " << this->serverAddress();
    }
}




void MyServer::myNewConnection()
{

        qDebug() << "Yaay one new client!";

}

//void MyServer::logError(QAbstractSocket::SocketError socketError)
//{

//    qDebug() << "error thrown: " << socketError;
//}




void MyServer::incomingConnection(qintptr socketDescriptor)
{
        // We have a new connection
        qInfo() << socketDescriptor << " Connecting...";

        MyThread *thread = new MyThread(socketDescriptor, this);

        // once a thread is not needed, it will be beleted later
        connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

        thread->start();
}
