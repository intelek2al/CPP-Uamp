#ifndef UTAG_SOUNDPLAYER_H
#define UTAG_SOUNDPLAYER_H
#include <QWidget>
#include <QMediaPlayer>
#include <QTime>
#include <QMediaPlaylist>
#include <QSqlTableModel>
#include <QSqlRecord>
#include "playerplaylist.h"

namespace Ui {
    class MainWindow;
}

class SoundPlayer : public QObject {
public:
    SoundPlayer(Ui::MainWindow *child);
    ~SoundPlayer();

    void setSound(QString path);
    void setSound(int index);

public slots:
    void playNext(const Music &song);
    void setPlay();
    void setPause();
    void setStop();
    void setVolume(int position);
    void setPosition(int position);
    void setMovedPosition(int position);
    void stateCheck(QMediaPlayer::State state);
    void setPlaylist(const Playlist &playlist);
    void setPlaylist(QSqlTableModel *model);
    void fastForward();
    void rewind();
    void metaData(bool check);
    static QMediaPlaylist *playlist();
//    static QMediaPlaylist *getInstance();
    void next();
    void previous();
    void savePlaylist(Playlist playlist, QString path = "/Users/msavytskyi/Desktop/");

private slots:

    void on_statusPlay_valueChanged(int value);
    void on_mainMusicTable_pressed(const QModelIndex &index);

private:
//    QMediaPlayer::State m_state = QMediaPlayer::State::StoppedState;
    QMediaPlayer *m_player;
    PlayerPlaylist m_list;
    static QMediaPlaylist *m_playlist;
    Ui::MainWindow *ui;
    bool isPlaylistExist = false;
};


#endif //UTAG_SOUNDPLAYER_H
