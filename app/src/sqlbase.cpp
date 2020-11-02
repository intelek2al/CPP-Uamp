#include "sqlbase.h"
#include <QtSql>
#include <QSqlDatabase>


SqlBase::SqlBase() {

    if (!createConnection()) {
      createNewBase();
    }
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
bool SqlBase::createNewBase() {
  // Создаем базу

  //    {"Title", "Time", "Artist", "Rating", "Genre", "Album", "Year", "Track", "Comment", "Name","Path"};

  /* Create SQL statement */
  QSqlQuery query;
  QString str = "CREATE TABLE SONGS ("  \
      "ID INT PRIMARY KEY     NOT NULL auto_increment ," \
      "Title          TEXT    NOT NULL," \
//      "Time            INT     NOT NULL," \ !!!!
      "Artist         TEXT    NOT NULL," \
      "Rating         TEXT    NOT NULL," \
      "Genre          TEXT    NOT NULL," \
      "Album          TEXT    NOT NULL," \
      "Year           TEXT    NOT NULL," \
      "Track          TEXT    NOT NULL," \
      "Comment        TEXT    NOT NULL," \
      "Name           TEXT    NOT NULL," \
      "Path           TEXT    NOT NULL);";

  if (!query.exec(str)) {
    qDebug() << "SQL error: %s\n";
  }
  else {
    qDebug() <<  "Table created successfully\n";
  }

    return false;
}
