#ifndef PLAYERPLAYLIST_H
#define PLAYERPLAYLIST_H

#include "playlist.h"
#include <QObject>

class PlayerPlaylist : public QObject
{
    Q_OBJECT
public:
    PlayerPlaylist();
    explicit PlayerPlaylist(const Playlist &playlist);
    void addUserNext(const Music &song);
    void clearCustomUser();
    void skipUser();
    void setPlaylist(const Playlist &playlist);
    void setStartSong(const Music &song);
    void setStartSong(const QUrl &song);
    void setStartSong(int pos);
    void setMode(QMediaPlaylist::PlaybackMode mode);
    Playlist &currentPlaylist();

public slots:
    void next();

private:
    void refresh();

private:
    QMediaPlaylist::PlaybackMode m_mode;
    Playlist m_history;
    Playlist m_user;
    Playlist m_auto;
    Playlist m_list;
    QMediaPlaylist *m_playlist;
};

#endif // PLAYERPLAYLIST_H