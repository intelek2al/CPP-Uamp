#include "sqlbase.h"
#include <QtSql>
#include <QSqlDatabase>
#include "loggingcategories.h"
#include "music.h"
#include "tag_functions.h"

//    {"Title", "Time", "Artist", "Rating", "Genre", "Album", "Year", "Track", "Comment", "Name","Path"};

SqlBase::SqlBase() {
    if (createConnection()) {
        qDebug(logDebug()) << "SqlBase: createConnection false";
        createNewBase();
    }
    else {
        qDebug(logDebug()) << "SqlBase: createConnection true";
        // exit from app !!!
    }
}

bool SqlBase::createNewBase() {
    bool result = createTableSongs();
    createTableListPlaylists();
    createTablePlaylist();
    return result;
}

bool SqlBase::createConnection() {

    qDebug(logDebug()) << "SqlBase::createConnection";
    m_media_base = QSqlDatabase:: addDatabase ("QSQLITE");
//    m_media_base.setDatabaseName("/Users/snikolayen/media_db.sqlite");
    m_media_base.setDatabaseName("/Users/Shared/media_db.sqlite");
    m_media_base.setUserName ("admin");
    m_media_base.setHostName("localhost");
    m_media_base. setPassword ("password");
    if (!m_media_base.open()) {
        qDebug(logDebug()) << "Cannot open database:" << m_media_base.lastError();
        return false;
    } else {
      QSqlQuery query;
      query.exec("PRAGMA foreign_keys = ON;");
    }
    return true;
}


bool SqlBase::createTableSongs() {
  qDebug(logDebug()) << "Create table SONGS";
  QSqlQuery query;
  QString str = "CREATE TABLE IF NOT EXISTS SONGS ("  \
      "SONG_ID             INTEGER PRIMARY KEY  AUTOINCREMENT," \
      "Title          TEXT    ," \
      "Time           TEXT    ," \
      "Artist         TEXT    ," \
      "Rating         TEXT    ," \
      "Genre          TEXT    ," \
      "Album          TEXT    ," \
      "Year           TEXT    ," \
      "Track          TEXT    ," \
      "Comment        TEXT    ," \
      "Name           TEXT    NOT NULL," \
      "Path           TEXT    UNIQUE  NOT NULL"  \
      ");";

  if (!query.exec(str)) {
    qDebug(logDebug()) << "Unable to create table SONGS" << query.lastError();
  }
  else {
    qDebug(logDebug()) << "Table SONGS created successfully\n";
  }
    return false;
}

bool SqlBase::createTableListPlaylists() {
    qDebug(logDebug()) << "Create table LIST_PLAYLISTS";

    QSqlQuery query;
    QString str = "CREATE TABLE IF NOT EXISTS LIST_PLAYLISTS ("  \
      "PLAY_LISTS_ID   INTEGER PRIMARY KEY  AUTOINCREMENT," \
      "Name            TEXT    UNIQUE NOT NULL " \
      ");";

    if (!query.exec(str)) {
        qDebug(logDebug()) << "Unable to create table LIST_PLAYLISTS" << query.lastError();
    }
    else {
        qDebug(logDebug()) << "Table LIST_PLAYLISTS created successfully\n";
    }
    return false;
}

bool SqlBase::createTablePlaylist() {
    qDebug(logDebug()) << "Create table PLAYLIST";

    QSqlQuery query;

    query.exec("PRAGMA foreign_keys = ON;");

    QString str = "CREATE TABLE IF NOT EXISTS PLAYLIST ("  \
      "ID             INTEGER  PRIMARY KEY  AUTOINCREMENT," \
      "PLAYLIST_R    INTEGER  NOT NULL," \
      "SONG_R        INTEGER  NOT NULL," \
      "FOREIGN KEY (PLAYLIST_R) REFERENCES LIST_PLAYLISTS (PLAY_LISTS_ID)" \
      "FOREIGN KEY (SONG_R) REFERENCES SONGS (SONG_ID) ON DELETE CASCADE" \
      ");";

    if (!query.exec(str)) {
        qDebug(logDebug()) << "Unable to create table PLAYLIST" << query.lastError();
    } else {
        qDebug(logDebug()) << "Table PLAYLIST created successfully\n";
    }
    return false;
}

bool SqlBase::loadData() {
    qDebug(logDebug()) <<  "SqlBase::loadData()";

    QSqlQuery query("SELECT Name FROM SONGS");
    while (query.next()) {
        QString country = query.value(0).toString();
//        doSomething(country);
    }
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

        QFileInfoList list = cur_dir2.entryInfoList();

        QSqlQuery query(m_media_base.connectionName());

        qDebug(logDebug()) << m_media_base.connectionName();

        for (int i = 0; i < list.size(); ++i)
        {
            qDebug (logDebug()) << "SqlBase::AddtoLibrary";

            Music curent_song = TagFunctions::LoadSongTags(list.at(i).filePath());
            if (!curent_song.empty()) {

                qDebug(logDebug()) << "Music current : " << curent_song.m_name, curent_song.m_title;
                query.prepare("INSERT INTO SONGS (Title, Time, Artist, Rating, Genre, Album, Year, "
                                                        "Track, Comment, Name, Path) "
                        "VALUES (:Title, :Time, :Artist, :Rating, :Genre, :Album, :Year, :Track, :Comment, "
                                ":Name, :Path)");
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

                if (!query.exec()) {
                    qDebug(logDebug()) << "error = " << query.lastError();
                }
            }
        }
    }
    return true;
}

SqlBase::~SqlBase() {

    m_media_base.close();
    m_media_base.removeDatabase(m_media_base.connectionName());
}

bool SqlBase::AddNewPlaylist(const QString &name) {
    qDebug(logDebug()) << "Create newPlaylist in LIST_PLAYLISTS";

    if (name.isEmpty()) {
        return false;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO LIST_PLAYLISTS (Name) "
                  "VALUES (:Name)");
    query.bindValue(":Name", name);

    if (!query.exec()) {
        qDebug(logDebug()) << "Create newPlaylist in LIST_PLAYLISTS failed, error = " << query.lastError();
        return false;
    }
    return true;
}

bool SqlBase::DeletePlaylist(const QString& name) {
    qDebug(logDebug()) << "Delete Playlist from LIST_PLAYLISTS";
    int PLAY_LISTS_R;
    QSqlQuery query;
    query.exec("PRAGMA foreign_keys = ON");

    query.prepare("SELECT PLAY_LISTS_ID FROM LIST_PLAYLISTS WHERE Name = ?");
    query.addBindValue(name);
    if (!query.exec()) {
        qDebug(logDebug()) << "error "  << query.lastError();
        return false;
    }
    query.next();
    PLAY_LISTS_R = query.value(0).toInt();

    query.prepare("DELETE FROM LIST_PLAYLISTS WHERE PLAY_LISTS_ID = ?");
    query.addBindValue(PLAY_LISTS_R);
    if (!query.exec()) {
        qDebug(logDebug()) << "error "  << query.lastError();
        return false;
    }

    query.prepare("DELETE FROM PLAYLIST WHERE PLAYLIST_R = ?");
    query.addBindValue(PLAY_LISTS_R);
    if (!query.exec()) {
        qDebug(logDebug()) << "error "  << query.lastError();
        return false;
    }
    return true;
}

bool SqlBase::AddtoPlaylist(const QString &path, const QString &cur_playlist) {
    qDebug(logDebug()) << "SqlBase::AddtoPlaylist";

    int PLAY_LISTS_R;
    int SONG_ID;

    if (path.isEmpty() || cur_playlist.isEmpty())
        return false;

    QSqlQuery query;
    query.prepare("SELECT PLAY_LISTS_ID FROM LIST_PLAYLISTS WHERE Name = ?");
    query.addBindValue(cur_playlist);

    if (!query.exec()) {
        qDebug(logDebug()) << "AddtoPlaylist failed, error = " << query.lastError();
        return false;
    }
    query.next();
    PLAY_LISTS_R = query.value(0).toInt();

    query.prepare("SELECT SONG_ID FROM SONGS WHERE Path = ?");
    query.addBindValue(path);
    if (!query.exec()) {
        qDebug(logDebug()) << "AddtoPlaylist failed, error = " << query.lastError();
        return false;
    }
    query.next();
    SONG_ID = query.value(0).toInt();

    query.prepare("INSERT INTO PLAYLIST (PLAYLIST_R, SONG_R) "
                  "VALUES (:PLAYLIST_R, :SONG_R)");
    query.bindValue(":PLAYLIST_R", PLAY_LISTS_R);
    query.bindValue(":SONG_R", SONG_ID);
    if (!query.exec()) {
        qDebug(logDebug()) << "AddtoPlaylist failed, error = " << query.lastError();
        return false;
    }
    return true;
}


/*
query.prepare("INSERT INTO PLAYLIST (PLAYLIST, SONG_ID) "
              "VALUES (:PLAYLIST, :SONG_ID)");
query.bindValue(":PLAYLIST", "SELECT FROM LIST_PLAYLISTS WHERE Name = ?");
query.addBindValue(cur_playlist);
query.bindValue(":SONG_ID", "SELECT FROM SONGS WHERE Path = ?");
query.addBindValue(path);
*/
