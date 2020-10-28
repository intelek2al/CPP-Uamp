#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <deque>
#include <QUrl>
#include <QMediaPlaylist>

#include "music.h"

class Playlist
{
public:
    Playlist();
    Playlist(QVector<QUrl> urls, const QString& name);
    void addMusic(const QUrl& url);
    void addMusic(const Music& music);
    void addFrontMusic(const QUrl& url);
    void addFrontMusic(const Music& music);
    void setPlaylistName(const QString& name);
    QString playlistName() const;
    Music getMusic(int pos);
    bool empty() const;
    void addToMediaPlaylist();
    void clearMusic(int pos);
    void clearMusic(const Music& music);
    void clear();
    size_t size() const;
    Music operator[](int index);

private:
    QString m_playlistName;
    std::deque<Music> m_musicPlaylist;
};

#endif // PLAYLIST_H