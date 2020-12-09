#include <QCoreApplication>
#include <myserver.h>
#include <QtDebug>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QDateTime>

QString logFileName;


/**
 * Search the configuration file.
 * Aborts the application if not found.
 * @return The valid filename
 */
QString searchConfigFile() {
    QString binDir=QCoreApplication::applicationDirPath();
    QString appName=QCoreApplication::applicationName();
    QFile file;
    file.setFileName(binDir+"/log.txt");
    if (!file.exists()) {
        file.setFileName(binDir+"/etc/log.txt");
        if (!file.exists()) {
            file.setFileName(binDir+"/../etc/log.txt");
            if (!file.exists()) {
                file.setFileName(binDir+"/../"+appName+"/etc/log.txt");
                if (!file.exists()) {
                    file.setFileName(binDir+"/../../"+appName+"/etc/log.txt");
                    if (!file.exists()) {
                        file.setFileName(binDir+"/../../../../../"+appName+"/etc/log.txt");
                        if (!file.exists()) {
                            file.setFileName(QDir::rootPath()+"etc/log.txt");
                        }
                    }
                }
            }
        }
    }
    if (file.exists()) {
        QString logFileName=QDir(file.fileName()).canonicalPath();
        qDebug("using log file %s", qPrintable(logFileName));
        return logFileName;
    }
    else {
        qFatal("log file not found");
    }
}




void myMessageHandler(QtMsgType type, const QMessageLogContext &, const QString & msg)
{
    QString txt;
    switch (type) {
    case QtDebugMsg:
        txt = QString("Debug: %1").arg(msg);
        break;
    case QtWarningMsg:
        txt = QString("Warning: %1").arg(msg);
    break;
    case QtCriticalMsg:
        txt = QString("Critical: %1").arg(msg);
    break;
    case QtFatalMsg:
        txt = QString("Fatal: %1").arg(msg);
    break;
    case QtInfoMsg:
        txt = QString("Info: %1").arg(msg);
    break;
    }


    //load datetime
    QDateTime dateTime = dateTime.currentDateTime();

    //choose and open file
//    QFile outFile("seyamLog");//creates new file in the mingw debug folder and writes data
    QFile outFile(logFileName);//predefined directory path

    outFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream ts(&outFile);
    ts << dateTime.toString("yyyy-MM-dd HH:mm:ss") << "   " << txt << Qt::endl;
}



int main(int argc, char *argv[])
{

    QCoreApplication a(argc, argv);

    // Load the configuration file
    logFileName = searchConfigFile();


    qInstallMessageHandler(myMessageHandler);

    MyServer server;

    server.startServer();

    return a.exec();
}
