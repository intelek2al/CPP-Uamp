#include "sqlbase.h"
#include <QtSql>
#include <QSqlDatabase>


SqlBase::SqlBase() {

    if (!createConnection()) {

    }
    QSqlDatabase dbase = QSqlDatabase::addDatabase("QSQLITE");

    dbase.setDatabaseName("media_db.sqlite");
    dbase.setUserName("master");
    dbase.setPassword("12345");

    if (!dbase.open()) {
        qDebug() << "Что-то пошло не так!";
//        return -1;
    }
//    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");

}

bool SqlBase::createConnection() {
    m_media_base = QSqlDatabase:: addDatabase ("QSQLITE");
    m_media_base.setDatabaseName("media_db.sqlite");
    m_media_base.setUserName ("admin");
//    m_media_base.setHostName("snikolayen");
    m_media_base. setPassword ("password");
    if (!m_media_base.open()) {
        qDebug() << "Cannot open database:" << m_media_base.lastError();
        return false;
    }
    return true;
}
