#include "playerplaylist.h"
#include "soundPlayer.h"

using namespace std;

PlayerPlaylist::PlayerPlaylist() {
    m_playlist = SoundPlayer::playlist();
    m_current_play = Auto;
}

PlayerPlaylist::PlayerPlaylist(const Playlist &playlist) {
    m_playlist = SoundPlayer::playlist();
    setPlaylist(playlist);
}

void PlayerPlaylist::addUserNext(const Music &song) {
//    if(m_user.empty()) {
//        m_user.addMusic(Music());
        m_user.addFrontMusic(song);
//    }
//    else {
//        m_user.addFrontMusic(song);
//    }
//    refresh();
}

void PlayerPlaylist::clearCustomUser() {
    m_user.clear();
}

void PlayerPlaylist::skipUser() {
    m_user.clear();
    refresh();
}

void PlayerPlaylist::setPlaylist(const Playlist &playlist) {
    m_user.clear();
    m_auto.clear();
    m_list = playlist;
//    m_auto = m_list;
}

#include <iostream>
void PlayerPlaylist::refresh() {
    m_playlist->clear();
    m_user.addToMediaPlaylist();
    m_auto.addToMediaPlaylist();
//    std::cout << " = = = = = = Playlist = = = = = = = \n";
//    std::cout << " : : : : User : : : : \n";
//    for (int i = 0; i < m_user.size(); i++) {
//        std::cout << m_user[i].getStr().toStdString() << std::endl;
//    }
//    std::cout << " : : : : Auto : : : : \n";
//    for (int i = 0; i < m_auto.size(); i++) {
//        std::cout << m_auto[i].getStr().toStdString() << std::endl;
//    }
}
#include <iostream>

void PlayerPlaylist::next() {
    if (m_current_play == Auto) {
        Music _tmp = m_auto[0];
        m_history.addFrontMusic(_tmp, Own::Auto);
        m_auto.clearMusic(0);
        m_auto.addMusic(_tmp);
    } else {
        m_history.addFrontMusic(m_user[0], Own::User);
        if (m_mode != QMediaPlaylist::CurrentItemInLoop && m_current_play == User) {
            m_user.clearMusic(0);
        }
    }
    if (!m_user.empty())
        m_current_play = User;
    else
        m_current_play = Auto;

    refresh();
}

void PlayerPlaylist::setMode(QMediaPlaylist::PlaybackMode mode) {
    m_mode = mode;
}

void PlayerPlaylist::setStartSong(const Music &song) {
//    if (!m_auto.empty())
//        m_history.addFrontMusic(m_auto[0], Auto);
    int pos = 0;
    for (; pos < m_list.size(); ++pos) {
        if (m_list[pos] == song)
            break;
    }
    if (pos == m_list.size())
        return;
    for (int i = 0; i < m_list.size(); ++i) {
        m_auto.addMusic(m_list[(pos++)]);
        pos = pos == m_list.size() ? 0 : pos;
    }
    m_playlist->clear();
    m_auto.addToMediaPlaylist();
}

void PlayerPlaylist::setStartSong(const QUrl &song) {
//    if (!m_auto.empty())
//        m_history.addFrontMusic(m_auto[0], Auto);
    int pos = 0;
    for (; pos < m_list.size(); ++pos) {
        if (m_list[pos].m_url == song)
            break;
    }
    if (pos == m_list.size())
        return;
    for (int i = 0; i < m_list.size(); ++i) {
        m_auto.addMusic(m_list[(pos++)]);
        pos = pos == m_list.size() ? 0 : pos;
    }
    m_playlist->clear();
    m_auto.addToMediaPlaylist();
}

Playlist &PlayerPlaylist::currentPlaylist() {
    return m_list;
}

void PlayerPlaylist::setStartSong(int pos) {
    if (!m_auto.empty())
        m_history.addFrontMusic(m_auto[0], Auto);
    m_auto.clear();
    for (int i = 0; i < m_list.size(); ++i) {
//        cout << "Mus added:" << m_list[pos].getStr().toStdString() << endl;
        m_auto.addMusic(m_list[(pos++)]);
        pos = pos == m_list.size() ? 0 : pos;
    }
    m_playlist->clear();
    m_auto.addToMediaPlaylist();
    m_playlist->setCurrentIndex(0);
//    if (m_playlist->error() == QMediaPlaylist::NoError)
//        cout << "Pos:" << pos << endl;
}

void PlayerPlaylist::setPlaybackMode(QMediaPlaylist::PlaybackMode mode) {
    m_mode = mode;
    switch (m_mode) {
        case QMediaPlaylist::PlaybackMode::Random: {
            m_auto.shuffle();
        }
        default:
            m_auto.unshuffle();
    }
    m_user.addToMediaPlaylist();
    m_auto.addToMediaPlaylist();
}

Playlist PlayerPlaylist::upNext() {
    Playlist upNext;
    upNext.addMusic(m_user);
    upNext.addMusic(m_auto);
    return upNext;
}

void PlayerPlaylist::previous() {
    Music _tmp;
    Own owner;
    if (m_history.history.empty()) {
        auto current = m_user.empty() ? m_auto : m_user;
        auto index = m_list.indexMusic(current[0]);
        index = index == 0 ? m_list.size() - 1 : index - 1;
        _tmp = m_list[index];
        owner = User;
    }
    else {
        _tmp = m_history.history[0];
        owner = m_history.historyOwners[0];
    }
    if (owner == Own::Auto)
        m_auto.addFrontMusic(_tmp);
    else if (owner == Own::User) {
        m_user.addFrontMusic(_tmp);
    }
    m_history.clearMusic(0);
    refresh();
}

Playlist PlayerPlaylist::history() {
    return m_history.history;
}

Music PlayerPlaylist::currentMusic() const {
    if (m_current_play == User)
        return m_user[0];
    return m_auto[0];
}

void PlayerPlaylist::setChangedPlaylist(const Playlist &playlist) {
    m_list = playlist;
    auto current = currentMusic();
    m_auto = m_list;
    if (m_mode == QMediaPlaylist::PlaybackMode::Random)
        m_auto.shuffle();
    if (m_current_play == Auto) {
        m_auto[0] = current;
    }

}
