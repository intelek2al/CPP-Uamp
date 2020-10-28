#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <vector>
#include <QUrl>
#include <QMediaPlaylist>

class Music;

class Playlist
{
public:
    Playlist();
    Playlist(QVector<QUrl> urls, const QString& name);
    void addMusic(const QUrl& url);
    void addMusic(const Music& music);
    void setPlaylistName(const QString& name);
    QString playlistName() const;
    Music getMusic(int pos);
    void addToPlaylist(QMediaPlaylist *playlist);
    void clearMusic();
    void clear();

private:
    QString m_playlistName;
    std::vector<Music> m_musicPlaylist;
};

#endif // PLAYLIST_H