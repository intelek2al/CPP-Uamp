#include "playerplaylist.h"
#include "soundPlayer.h"

PlayerPlaylist::PlayerPlaylist() {
    m_playlist = SoundPlayer::playlist();
}

PlayerPlaylist::PlayerPlaylist(const Playlist &playlist) {
    m_playlist = SoundPlayer::playlist();
    m_auto = playlist;
}

void PlayerPlaylist::addUserNext(const Music &song) {
    m_user.addMusic(song);
    refresh();
}

void PlayerPlaylist::clearCustomUser() {
    m_user.clear();
}

void PlayerPlaylist::skipUser() {
    m_user.clear();
    refresh();
}

void PlayerPlaylist::setPlaylist(const Playlist &playlist) {
    m_auto = playlist;
}

void PlayerPlaylist::refresh() {
    m_playlist->clear();
    m_user.addToMediaPlaylist();
    m_auto.addToMediaPlaylist();
}

void PlayerPlaylist::next() {
    if (!m_user.empty()) {
        m_history.addFrontMusic(m_user[0]);
        if (m_mode != QMediaPlaylist::CurrentItemInLoop)
            m_user.clearMusic(0);
        return;
    }

    m_history.addFrontMusic(m_auto[0]);
    m_auto.clearMusic(0);
    return;
}

void PlayerPlaylist::setMode(QMediaPlaylist::PlaybackMode mode) {
    m_mode = mode;
}


