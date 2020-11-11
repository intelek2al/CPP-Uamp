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

class MainWindow;

class SoundPlayer : public QObject {
    Q_OBJECT
public:
    SoundPlayer(Ui::MainWindow *child, MainWindow *parent);
    ~SoundPlayer();

    void setSound(QString path);
    void setSound(int index, QSqlQueryModel *model = nullptr);
    void next();
    void changeMode();
    Music getMusicfromTable(QModelIndex index) const;
    int volume() const;

signals:
    void playlistImported(Playlist import_playlist);

public slots:
    void playNext(const Music &song);
    void setPlay();
    void setPause();
    void setStop();
    void setVolume(int position);
    void setPosition(int position);
    void setMovedPosition(int position);
    void setPlaylist(const Playlist &playlist);
    void setPlaylist(QSqlQueryModel *model);
    void fastForward();
    void rewind();
    void metaData(bool check);
    static QMediaPlaylist *playlist();
//    static QMediaPlaylist *getInstance();
    void autoNext(QMediaPlayer::MediaStatus status);
    void previous();
    void modelChanged();
    Playlist upNext();
    Playlist history() const;
    void importPlaylist(const QString &path);
    void exportPlaylist(const Playlist &playlist = Playlist(), QString path = "/Users/msavytskyi/Desktop/");

    static Playlist handlerPlaylist(QSqlQueryModel *model);

private slots:
//    void on_statusPlay_valueChangedålay_valueChanged(int value);
//    void on_mainMusicTable_pressed(const QModelIndex &index);

private:
//    QMediaPlayer::State m_state = QMediaPlayer::State::StoppedState;
    QMediaPlaylist::PlaybackMode m_mode;
    QMediaPlayer *m_player;
    PlayerPlaylist m_list;
    MainWindow *m_parent;
    static QMediaPlaylist *m_playlist;
    Ui::MainWindow *ui;
    bool isPlaylistExist = false;
    QSqlQueryModel *m_model;

};


#endif //UTAG_SOUNDPLAYER_H
