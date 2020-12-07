#include "mythread.h"



MyThread::MyThread(qintptr ID,   QObject *parent) : QThread(parent)
{
    this->socketDescriptor = ID;
}

void MyThread::run()
{
    // thread starts here
    qDebug() << " Thread started";


    socket = new QTcpSocket();

    //connect signal and slot
    // note - Qt::DirectConnection is used because it's multithreaded

    connect(socket, SIGNAL(readyRead()),    this,   SLOT( readyRead()), Qt::DirectConnection);
    connect(socket, SIGNAL(disconnected()),    this,   SLOT( disconnected()));
//    connect(socket, SIGNAL(error(QTcpSocket::SocketError socketerror)),    this,   SLOT( errorThrown()));
    connect(socket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::errorOccurred),  this, &MyThread::errorThrown);

    if(!socket->setSocketDescriptor(this->socketDescriptor)){
        emit error(socket->error());
        return;
    }




    // We'll have multiple clients, we want to know which is which
    qDebug() << "Client "<< socketDescriptor << "  connected";

    // make this thread a loop,
    // thread will stay alive so that signal/slot to function properly
    // not dropped out in the middle when thread dies

    exec();

}



void MyThread::readyRead()
{
    QByteArray data = socket->readAll();

    // will write on server side window
    qDebug() << socketDescriptor << " Data in: " << data;

//    qDebug() << data.at(0);
//    QByteArray auth_token = "FF0186000000000079";
//    QByteArray data_token = "FF0186000000000079";
    QByteArray secretData = "FF86006500360096019A0002FD002800280005002000500000EA";

    if(!data.compare(auth_token,Qt::CaseInsensitive)){
        qDebug() << "Client Authorized";
        socket->write("200");
        //also keep the socket open for further communication

    }else if(!data.compare(data_token,Qt::CaseInsensitive)){
            qDebug() << "Token Authorized For Sending Data ";
            qDebug() << "Sending data: " << secretData;
            socket->write(secretData);
            //now the socket can be closed or kept it open for any acknowledgement return from client
    }
    else{
        qDebug() << "Client Not Authorized";
        socket->write("400");
        //since the client is not authorized, so close the connection
        socket->close();
//        socket->deleteLater();    //this is causing server crash if uninteded/unexpected data received, later i found that if this function is  being called here, then when socket disconnects inside disconnect slot,
        // it is again being called there for clean-up  but it is already destroyed here causing a null pointer exception there.

    }



//    QByteArray dt = "MIZAN BHAI VALO ASEN?";

    //echo back to the client
//    socket->write(data);

}

void MyThread::disconnected()
{
    qWarning() << socketDescriptor << " Disconnected";

    socket->deleteLater();//safe here, no crash
    exit(0);
}



void MyThread::errorThrown(QAbstractSocket::SocketError socketerror)
{
    qDebug() << "error: " << socketerror;
}




