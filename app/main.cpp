#include <iomanip>
#include <iostream>
#include <string>

#include "src/mainwindow.h"
#include "src/loggingcategories.h"
#include "src/app.h"

#include <sys/stat.h>
#include <dirent.h>
#include <QApplication>
#include <QPushButton>
#include <QGridLayout>
#include <QLineEdit>
#include <QDateTimeEdit>
#include <QSpinBox>
#include <QComboBox>
#include <QLabel>
#include <QStringList>
#include <QDir>
//#include <Q

QScopedPointer<QFile>   m_logFile;

using std::cout;

void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);

int main(int argc, char **argv)
{
    App app(argc, argv, "Ucode", "uamp");

    m_logFile.reset(new QFile("../../app/logFile.txt"));  // run from Clion
    m_logFile.data()->open(QFile::Append | QFile::Text);
    // Устанавливаем обработчик. To restore the message handler, call qInstallMessageHandler(0).
    qInstallMessageHandler(messageHandler);

//    QSqlDatabase dbase = QSqlDatabase::addDatabase("QSQLITE");
//    dbase.setDatabaseName("my_db.sqlite");
//    if (!dbase.open()) {
//        qDebug() << "Что-то пошло не так!";
//        return -1;
//    }


    MainWindow window;
    window.show();
//    system("leaks -q uamp");

    return app.exec();
}

void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QTextStream out(m_logFile.data());

    out << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz ");
    QByteArray localMsg = msg.toLocal8Bit();
    const char *file = context.file ? context.file : "";
    const char *function = context.function ? context.function : "";

    switch (type) {
        case QtDebugMsg:
            out << QString(" Debug: %1 (%2:%3, %4)\n").arg(localMsg.constData()).arg(file).arg(context.line).arg(function);
            break;
        case QtInfoMsg:
            out << QString(" Info: %1 (%2:%3, %4)\n").arg(localMsg.constData()).arg(file).arg(context.line).arg(function);
//            out << QString(" Info: %1)\n").arg(localMsg.constData());
            break;
        case QtWarningMsg:
            out << QString(" Warning: %1 (%2:%3, %4)\n").arg(localMsg.constData()).arg(file).arg(context.line).arg(function);
            break;
        case QtCriticalMsg:
            out << QString(" Critical: %1 (%2:%3, %4)\n").arg(localMsg.constData()).arg(file).arg(context.line).arg(function);
            break;
        case QtFatalMsg:
            out << QString(" Fatal: %1 (%2:%3, %4)\n").arg(localMsg.constData()).arg(file).arg(context.line).arg(function);
            break;
    }
    out.flush();    // Очищаем буферизированные данные
}
