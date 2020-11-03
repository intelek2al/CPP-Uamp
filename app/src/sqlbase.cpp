#include "sqlbase.h"
#include <QtSql>
#include <QSqlDatabase>
#include "loggingcategories.h"
#include "music.h"
#include "tag_functions.h"


SqlBase::SqlBase() {

    if (!createConnection()) {
        qDebug(logDebug()) << "SqlBase: createConnection false";

    }
    createNewBase();

    qDebug(logDebug()) << "SqlBase: createConnection true";
}

bool SqlBase::createConnection() {
//    QString prefix = "~/";

    qDebug(logDebug()) << "SqlBase::createConnection";
    m_media_base = QSqlDatabase:: addDatabase ("QSQLITE");
    m_media_base.setDatabaseName("/Users/snikolayen/media_db.sqlite");
    m_media_base.setUserName ("admin");
    m_media_base.setHostName("localhost");
    m_media_base. setPassword ("password");
    if (!m_media_base.open()) {
        qDebug(logDebug()) << "Cannot open database:" << m_media_base.lastError();
        return false;
    }
    return true;
}
bool SqlBase::createNewBase() {
  // Создаем базу

  //    {"Title", "Time", "Artist", "Rating", "Genre", "Album", "Year", "Track", "Comment", "Name","Path"};

  /* Create SQL statement */
  qDebug(logDebug()) << "Create table SONGS";
  QSqlQuery query;
  QString str = "CREATE TABLE IF NOT EXISTS SONGS ("  \
      "ID INTEGER PRIMARY KEY   NOT NULL ," \
      "Title          TEXT    NOT NULL," \
      "Time           TEXT     NOT NULL," \
      "Artist         TEXT    ," \
      "Rating         TEXT    ," \
      "Genre          TEXT    ," \
      "Album          TEXT    ," \
      "Year           TEXT    ," \
      "Track          TEXT    ," \
      "Comment        TEXT    ," \
      "Name           TEXT    NOT NULL," \
      "Path     TEXT  UNIQUE  NOT NULL"  \
              ");";


  if (!query.exec(str)) {
    qDebug(logDebug()) << "Unable to create table" << query.lastError();
  }
  else {
    qDebug(logDebug()) <<  "Table created successfully\n";
  }

    return false;
}

bool SqlBase::loadData() {
    qDebug(logDebug()) <<  "SqlBase::loadData()";

    return true;
}

bool SqlBase::AddtoLibrary(const QString &media_path) {
    if (QDir cur_dir(media_path); !cur_dir.isReadable())
    {
        qWarning(logWarning()) << QString(QDir(media_path).dirName() + " directory not readable");
    }
    else {
        qDebug (logDebug()) << "SqlBase::AddtoLibrary";
        QDir cur_dir2(media_path);

        cur_dir2.setFilter(QDir::NoDotAndDotDot | QDir::Files);
        cur_dir2.setNameFilters(QStringList() << "*.mp3"
                                             << "*.flac"
                                             << "*.wav"
                                             << "*.ogg"
                                             << "*.m4a"
                                             << "*.aif");
        //    current_directory.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
        //    current_directory.setSorting(QDir::Size | QDir::Reversed);

        QFileInfoList list = cur_dir2.entryInfoList();

        QSqlQuery query;
        for (int i = 0; i < list.size(); ++i)
        {

        Music curent_song = LoadSongTags(list.at(i).filePath());
        query.prepare("INSERT INTO SONGS (Title, Time, Artist, Rating, Genre, Album, Year, Track, Comment, Name, Path) "
                      "VALUES (:forename, :surname)");
//        query.bindValue(":id", 1001);
        query.bindValue(":Title", curent_song.m_title);
        query.bindValue(":Time", curent_song.m_time);
        query.bindValue(":Artist", curent_song.m_artist);
        query.bindValue(":Time", curent_song.m_time);
        query.bindValue(":Rating", "0");
        query.bindValue(":Genre", curent_song.m_genre);
        query.bindValue(":Album", curent_song.m_album);
        query.bindValue(":Year", curent_song.m_year);
        query.bindValue(":Track", curent_song.m_track);
        query.bindValue(":Comment", curent_song.m_comment);
        query.bindValue(":Name", curent_song.m_name);
        query.bindValue(":Path", curent_song.m_path);

        query.exec();
//            add_file(list.at(i).filePath());
        }
    }
    return true;
}


Music SqlBase::LoadSongTags(const QString &file_name) {

    QFileInfo fileInfo(file_name);
    if (!fileInfo.isReadable()) {
        qWarning(logWarning()) << fileInfo.fileName() << " not readable";
    }
    Music tmp;
    try
    {
//            Sound_tags current;
        tmp = TagFunctions::read_tags(TagFunctions::toChar(QString(fileInfo.fileName())),
                                      TagFunctions::toChar(QString(fileInfo.filePath())));
    }
    catch (std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
    if (!tmp.empty())
        return tmp;
}
