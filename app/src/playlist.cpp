#include "playlist.h"
#include "music.h"
#include "tag_functions.h"

static char *toChar(QString str)
{
    char *test = str.toUtf8().data();
    return test;
}

Playlist::Playlist()
{

}

Playlist::Playlist(QVector<QUrl> urls, const QString &name) {
    for(const auto &url : urls) {
        m_musicPlaylist.push_back(url);
    }
}

void Playlist::addMusic(const QUrl &url) {
    Music song = read_tags(toChar(url.fileName()), toChar(url.toLocalFile()));
    m_musicPlaylist.push_back(std::move(song));
}

void Playlist::addMusic(const Music &music) {
    m_musicPlaylist.push_back(music);
}

void Playlist::setPlaylistName(const QString &name) {
    m_playlistName = name;
}

QString Playlist::playlistName() const {
    return m_playlistName;
}

Music Playlist::getMusic(int pos) {
    if(pos >= 0 && pos < m_musicPlaylist.size())
        return m_musicPlaylist[pos];
    return Music();
}

void Playlist::addToPlaylist(QMediaPlaylist *playlist) {
    for(const auto &song : m_musicPlaylist) {
        playlist->addMedia(song.m_url);
    }
}


