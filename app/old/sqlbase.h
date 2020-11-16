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
   bool AddtoLibrary(const QStringList& files);
   bool AddNewPlaylist(const QString& name);
   bool DeletePlaylist(const QString& name);
   Playlist ExportPlaylist(const QString& name);
   bool AddtoPlaylist(const QString& path, const QString& cur_playlist);

signals:
    void modelPlaylistSelect();
    void modelMusicSelect();

public slots:
    bool insertIntoTable(const Music& curent_song);      // Добавление записей в таблицу
    bool updateTableRow(const QModelIndex &index, const Music &new_tags);
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
