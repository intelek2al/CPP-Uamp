#ifndef UAMP_SQLBASE_H
#define UAMP_SQLBASE_H

#include <QObject>
#include <QString>
#include <QSqlDatabase>
#include "music.h"
#include "playlist.h"

class SqlBase : public QObject {
    Q_OBJECT

public:
   SqlBase();
   ~SqlBase();

   bool AddtoLibrary(const QString& media_path);
   bool AddNewPlaylist(const QString& name);
   bool DeletePlaylist(const QString& name);
   Playlist ExportPlaylist(const QString& name);
   bool AddtoPlaylist(const QString& path, const QString& cur_playlist);

signals:
    void modelPlaylistSelect();

public slots:
    bool insertIntoTable(const Music& curent_song);      // Добавление записей в таблицу
//    bool insertIntoTable(const QString &name, const QByteArray &pic);
    bool importPlayList(Playlist import_playlist);

private:
    bool createConnection();
    bool createNewBase();
    bool createTableSongs();
    bool createTableListPlaylists();
    bool createTablePlaylist();
    bool loadData();
    QSqlDatabase m_media_base;
};


#endif //UAMP_SQLBASE_H
