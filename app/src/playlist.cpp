#include "playlist.h"
#include "tag_functions.h"
#include "soundPlayer.h"

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
    m_playlistName = name;
}

void Playlist::addMusic(const QUrl &url) {
    Music song = TagFunctions::read_tags(toChar(url.fileName()), toChar(url.path()));
    m_musicPlaylist.push_back(std::move(song));
}

void Playlist::addMusic(const Music &music) {
    m_musicPlaylist.push_back(music);
//    std::cout << music.m_url.path().toStdString() <<std::endl;
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

void Playlist::addToMediaPlaylist() {
    for(const auto &song : m_musicPlaylist) {
        SoundPlayer::playlist()->addMedia(QUrl::fromLocalFile(song.m_path));
    }
}

void Playlist::clearMusic(int pos) {
    int i = 0;
    for(auto it = m_musicPlaylist.begin(); it != m_musicPlaylist.end();){
        if(i == pos){
            it = m_musicPlaylist.erase(it);
            break;
        }
        else{
            it++;
        }
        ++i;
    }
}

void Playlist::clearMusic(const Music& music) {
    for(auto it = m_musicPlaylist.begin(); it != m_musicPlaylist.end();){
        if(*it == music){
            it = m_musicPlaylist.erase(it);
            break;
        }
        else{
            it++;
        }
    }
}

void Playlist::clear() {
    m_musicPlaylist.clear();
}

bool Playlist::empty() const {
    return m_musicPlaylist.empty();
}

Music Playlist::operator[](int index) {
    if (index > 0 && index < m_musicPlaylist.size())
        return m_musicPlaylist[index];
    return Music();
}

void Playlist::addFrontMusic(const QUrl &url) {
    Music song = TagFunctions::read_tags(toChar(url.fileName()), toChar(url.path()));
    m_musicPlaylist.push_front(std::move(song));
}

void Playlist::addFrontMusic(const Music &music) {
    m_musicPlaylist.push_front(std::move(music));
}

size_t Playlist::size() const {
    return m_musicPlaylist.size();
}

