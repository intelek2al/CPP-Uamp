#ifndef UAMP_SINGLETONBASE_H
#define UAMP_SINGLETONBASE_H

#include <QObject>
#include <QString>
#include <QSqlDatabase>
#include "music.h"
#include "playlist.h"

class SingletonBase;

class SingletonDestroyer {

private:
    SingletonBase* p_instance;


public:
    ~SingletonDestroyer();
    void initialize(SingletonBase* p);
};


class SingletonBase : public QObject {

Q_OBJECT

private:
    static SingletonBase* p_instance;
    static SingletonDestroyer destroyer;


    QSqlDatabase m_media_base;
    bool createConnection();
    bool createNewBase();
    bool createTableSongs();
    bool createTableListPlaylists();
    bool createTablePlaylist();
    bool loadData();




protected:
    SingletonBase();
    SingletonBase (const  SingletonBase& );
    SingletonBase& operator=(SingletonBase& );
    ~SingletonBase() { }
    friend class SingletonDestroyer;
public:
    static SingletonBase& getInstance();

    bool AddtoLibrary(const QString& media_path);
    bool AddtoLibrary(const QStringList& files);
    bool AddNewPlaylist(const QString& name);
    bool DeletePlaylist(const QString& name);
    Playlist ExportPlaylist(const QString& name);
    bool AddtoPlaylist(const QString& path, const QString& cur_playlist);
    bool closeDataBase();

signals:
    void modelPlaylistSelect();
    void modelMusicSelect();

public slots:
    bool insertIntoTable(const Music& curent_song);      // Добавление записей в таблицу
    bool updateTableRow(const QModelIndex &index, const Music &new_tags);
    bool importPlayList(Playlist import_playlist);
};


#endif //UAMP_SINGLETONBASE_H
