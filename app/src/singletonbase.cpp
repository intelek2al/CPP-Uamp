#include "singletonbase.h"
#include <QtSql>
#include <QSqlDatabase>
#include "loggingcategories.h"
#include "music.h"
#include "tag_functions.h"

SingletonBase * SingletonBase::p_instance = 0;

SingletonDestroyer SingletonBase::destroyer;

SingletonDestroyer::~SingletonDestroyer() noexcept {
    qDebug(logDebug()) << "SingletonDestroyer::~SingletonDestroyer";

//    delete p_instance;
}


void SingletonDestroyer::initialize(SingletonBase *p) {
    p_instance = p;
}

SingletonBase& SingletonBase::getInstance() {
    qDebug(logDebug()) << "SingletonBase::getInstance";
    if (!p_instance) {
        p_instance = new SingletonBase();
        destroyer.initialize(p_instance);
    }
    return *p_instance;
}

SingletonBase::SingletonBase() {
    if (createConnection()) {
        qDebug(logDebug()) << "SqlBase: createConnection false";
        createNewBase();
    }
    else {
        qDebug(logDebug()) << "SqlBase: createConnection true";
        // exit from app !!!
    }
}


bool SingletonBase::createNewBase() {
    bool result = createTableSongs();
    createTableListPlaylists();
    createTablePlaylist();
    return result;
}

bool SingletonBase::createConnection() {

    qDebug(logDebug()) << "SqlBase::createConnection";
    m_media_base = QSqlDatabase:: addDatabase ("QSQLITE");
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


bool SingletonBase::createTableSongs() {
    qDebug(logDebug()) << "Create table SONGS";
    QSqlQuery query;
    QString str = "CREATE TABLE IF NOT EXISTS SONGS ("  \
      "SONG_ID        INTEGER PRIMARY KEY  AUTOINCREMENT," \
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
      "Path           TEXT    UNIQUE  NOT NULL,"  \
      "Lyrics         TEXT    ,"  \
      "Count          INTEGER ," \
      "Cover          BLOB    "  \
      ");";

    if (!query.exec(str)) {
        qDebug(logDebug()) << "Unable to create table SONGS" << query.lastError();
    }
    else {
        qDebug(logDebug()) << "Table SONGS created successfully\n";
    }
    return false;
}

bool SingletonBase::createTableListPlaylists() {
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

bool SingletonBase::createTablePlaylist() {
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

bool SingletonBase::loadData() {
    qDebug(logDebug()) <<  "SqlBase::loadData()";

    QSqlQuery query("SELECT Name FROM SONGS");
    while (query.next()) {
        QString country = query.value(0).toString();
//        doSomething(country);
    }
    return true;
}

bool SingletonBase::AddtoLibrary(const QString &media_path) {
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

        for (int i = 0; i < list.size(); ++i) {
            qDebug (logDebug()) << "SqlBase::AddtoLibrary";
            Music curent_song = TagFunctions::LoadSongTags(list.at(i).filePath());
            if (!insertIntoTable(curent_song)) {
                qDebug(logDebug()) << "Insert into base failed";
            }
        }
    }
    return true;
}

bool SingletonBase::AddtoLibrary(const QStringList &files) {
    qDebug(logDebug()) << "SqlBase::AddtoLibrary";

    QSqlQuery query(m_media_base.connectionName());

    for (int i = 0; i < files.size(); ++i) {
        Music curent_song = TagFunctions::LoadSongTags(files.at(i));
        if (!insertIntoTable(curent_song)) {
            qDebug(logDebug()) << "Insert into base failed";
        }
    }
    return true;
}


bool SingletonBase::insertIntoTable(const Music &curent_song) {
    QSqlQuery query;
    if (!curent_song.empty()) {
        qDebug(logDebug()) << "Music current : " << curent_song.m_name, curent_song.m_title;

        query.prepare("INSERT INTO SONGS (Title, Time, Artist, Rating, Genre, Album, Year, "
                      "Track, Comment, Name, Lyrics, Path, Cover) "
                      "VALUES (:Title, :Time, :Artist, :Rating, :Genre, :Album, :Year, :Track, :Comment, "
                      ":Name, :Lyrics, :Path, :Cover)");
        query.bindValue(":Title", curent_song.m_title);
        query.bindValue(":Time", curent_song.m_time);
        query.bindValue(":Artist", curent_song.m_artist);
        query.bindValue(":Rating", "0");
        query.bindValue(":Genre", curent_song.m_genre);
        query.bindValue(":Album", curent_song.m_album);
        query.bindValue(":Year", curent_song.m_year);
        query.bindValue(":Track", curent_song.m_track);
        query.bindValue(":Comment", curent_song.m_comment);
        query.bindValue(":Name", curent_song.m_name);
        query.bindValue(":Lyrics", curent_song.m_lyrics);
        query.bindValue(":Path", curent_song.m_path);
        query.bindValue(":Cover", curent_song.m_cover);
        query.bindValue(":Count", curent_song.m_count);

        if (!query.exec()) {
            qDebug(logDebug()) << "error = " << query.lastError();
            return false;
        }
    }
    return true;
}



bool SingletonBase::AddNewPlaylist(const QString &name) {
    qDebug(logDebug()) << "SqlBase::AddNewPlaylist in LIST_PLAYLISTS";

    if (name.isEmpty()) {
        qDebug(logDebug()) << "SqlBase::AddNewPlaylist name is Empty";
        return false;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO LIST_PLAYLISTS (Name) "
                  "VALUES (:Name)");
    query.bindValue(":Name", name);

    if (!query.exec()) {
        qDebug(logDebug()) << "SqlBase::AddNewPlaylist Create newPlaylist in LIST_PLAYLISTS failed, error = " << query.lastError();
        return false;
    }
    return true;
}

bool SingletonBase::DeletePlaylist(const QString& name) {
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

    query.prepare("DELETE FROM PLAYLIST WHERE PLAYLIST_R = ?");
    query.addBindValue(PLAY_LISTS_R);
    if (!query.exec()) {
        qDebug(logDebug()) << "error "  << query.lastError();
        return false;
    }

    query.prepare("DELETE FROM LIST_PLAYLISTS WHERE PLAY_LISTS_ID = ?");
    query.addBindValue(PLAY_LISTS_R);
    if (!query.exec()) {
        qDebug(logDebug()) << "error "  << query.lastError();
        return false;
    }

    return true;
}

bool SingletonBase::AddtoPlaylist(const QString &path, const QString &cur_playlist) {
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

Playlist SingletonBase::ExportPlaylist(const QString &name) {
    qInfo(logInfo()) <<"SqlBase::ExportPlaylist";
    int PLAY_LISTS_R;

    if (name.isEmpty())
        return Playlist{};

    QSqlQuery query;
    query.prepare("SELECT PLAY_LISTS_ID FROM LIST_PLAYLISTS WHERE Name = ?");
    query.addBindValue(name);
    if (!query.exec()) {
        qDebug(logDebug()) << "error "  << query.lastError();
        return Playlist{};
    }
    query.next();
    PLAY_LISTS_R = query.value(0).toInt();
    qInfo(logInfo()) << "PLAY_LISTS_R =" << PLAY_LISTS_R;

    query.prepare("SELECT SONGS.SONG_ID, SONGS.Path FROM SONGS INNER JOIN PLAYLIST ON SONGS.SONG_ID = PLAYLIST.SONG_R WHERE PLAYLIST.PLAYLIST_R = ?");
    query.addBindValue(PLAY_LISTS_R);
    if (!query.exec()) {
        qDebug(logDebug()) << "ExportPlaylist error ="  << query.lastError();
        return Playlist{};

    }
    Playlist export_playlist();
    QVector<QUrl> urls;
    while (query.next()) {
        QString song_path = query.value("Path").toString();
        urls.push_back(QUrl::fromLocalFile(song_path));
    }
    return {urls, name};
}


bool SingletonBase::importPlayList(Playlist import_playlist) {
    qDebug(logDebug()) << "SqlBase::ImportPlayList name = " << import_playlist.playlistName();

    if (!(this->AddNewPlaylist(import_playlist.playlistName()))) {
        return false;
    }

    emit modelPlaylistSelect();
    for (int i = 0; i < import_playlist.size(); ++i) {
        if (!import_playlist[i].empty()) {
            this->insertIntoTable(import_playlist[i]);
            this->AddtoPlaylist(import_playlist[i].m_path, import_playlist.playlistName());
        }
        else {
            return false;
        }
    }
    return true;
}

bool SingletonBase::updateTableRow(const QModelIndex &index, const Music &new_tags) {
    qDebug(logDebug()) << "SqlBase::updateTableRow";
    QSqlQuery query;
    int current_song_id;

    if (!new_tags.empty()) {
        query.prepare("SELECT SONG_ID FROM SONGS WHERE Path = ?");
        query.addBindValue(new_tags.m_path);

        if (!query.exec()) {
            qDebug(logDebug()) << "SqlBase::updateTableRow error "  << query.lastError();
            return false;
        }
        query.next();
        current_song_id = query.value(0).toInt();

        query.prepare("UPDATE SONGS SET Title = :Title, Time =:Time, Artist = :Artist,"
                      "Rating = :Rating, Genre = :Genre, Album = :Album, Year = :Year, Track = :Track,"
                      "Comment = :Comment, Cover =:Cover, Lyrics =:Lyrics "
                      "WHERE SONG_ID = :SONG_ID ");

        query.bindValue(":Title", new_tags.m_title);
        query.bindValue(":Time", new_tags.m_time);
        query.bindValue(":Artist", new_tags.m_artist);
        query.bindValue(":Rating", new_tags.m_rate);
        query.bindValue(":Genre", new_tags.m_genre);
        query.bindValue(":Album", new_tags.m_album);
        query.bindValue(":Year", new_tags.m_year);
        query.bindValue(":Track", new_tags.m_track);
        query.bindValue(":Comment", new_tags.m_comment);
        query.bindValue(":Cover", new_tags.m_cover);
        query.bindValue(":Count", new_tags.m_count);
        query.bindValue(":Lyrics", new_tags.m_lyrics);
        query.bindValue(":SONG_ID", current_song_id);

        if (!query.exec()) {
            qDebug(logDebug()) << "error = " << query.lastError();
            return false;
        }
        emit modelMusicSelect();
    }
    return true;
}

bool SingletonBase::closeDataBase() {
    qDebug(logDebug()) << "SingletonBase::closeDataBase";
    m_media_base.close();
//    m_media_base.removeDatabase(m_media_base.connectionName());
    return false;
}

