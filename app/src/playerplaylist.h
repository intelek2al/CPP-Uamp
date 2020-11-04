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
    Playlist upNext();
    Playlist history();

public slots:
    void next();
    void previous();
    void setPlaybackMode(QMediaPlaylist::PlaybackMode mode);


private:
    enum Own {
        User, Auto
    };
    struct History {
        Playlist history;
        std::deque<Own> historyOwners;
        void addFrontMusic(const Music &music, Own owner) {
            history.addFrontMusic(music);
            historyOwners.push_front(owner);
        }

        void clearMusic(int pos) {
            history.clearMusic(0);
            if (history.empty())
                return;
            if (pos == 0) {
                historyOwners.pop_front();
                return;
            } else if (pos == historyOwners.size() - 1) {
                historyOwners.pop_back();
                return;
            }
            int i = 0;
            for(auto it = historyOwners.begin(); it != historyOwners.end();){
                if(i == pos){
                    it = historyOwners.erase(it);
                    break;
                }
                else{
                    it++;
                }
                ++i;
            }
        }
    };
    void refresh();
private:
    QMediaPlaylist::PlaybackMode m_mode;
    History m_history;
    Playlist m_user;
    Playlist m_auto;
    Playlist m_list;
    QMediaPlaylist *m_playlist;
};

#endif // PLAYERPLAYLIST_H