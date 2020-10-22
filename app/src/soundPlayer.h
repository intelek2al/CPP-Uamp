#ifndef UTAG_SOUNDPLAYER_H
#define UTAG_SOUNDPLAYER_H
#include <QWidget>
#include <QMediaPlayer>
#include <QTime>

namespace Ui {
    class MainWindow;
}

class SoundPlayer : public QObject {
public:
    SoundPlayer(Ui::MainWindow *child);
    ~SoundPlayer();

    void setSound(QString path);

public slots:
    void setPlay();
    void setPause();
    void setStop();
    void setVolume(int position);
    void setPosition(int position);
    void setMovedPosition(int position);
    void stateCheck(QMediaPlayer::State state);

private slots:
    void on_statusPlay_valueChanged(int value);
    void on_mainMusicTable_pressed(const QModelIndex &index);

private:
    QMediaPlayer *m_player;
    Ui::MainWindow *ui;
};


#endif //UTAG_SOUNDPLAYER_H
