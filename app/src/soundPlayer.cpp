#include "soundPlayer.h"
#include "ui_mainwindow.h"

SoundPlayer::SoundPlayer(Ui::MainWindow *child)
{
    m_player = new QMediaPlayer();
    ui = child;
    connect(m_player, &QMediaPlayer::positionChanged, this, &SoundPlayer::setPosition);
    connect(m_player, &QMediaPlayer::stateChanged, this, &SoundPlayer::stateCheck);
    ui->labelSong->setText("");
    ui->labelArtist->setText("");
}

SoundPlayer::~SoundPlayer()
{
    delete m_player;
}

#include <QMediaMetaData>

void SoundPlayer::setSound(QString path)
{
    m_player->setMedia(QUrl::fromLocalFile(path));
    if (m_player->isMetaDataAvailable())
        ui->statusbar->showMessage("Audio Avalib ", 2000);
    else {
        while (!m_player->isMetaDataAvailable()) {
            QCoreApplication::processEvents();
        }
    }
//    QStringList list = m_player->availableMetaData();

    this->setPlay();
    ui->playButton->click();
}

void SoundPlayer::stateCheck(QMediaPlayer::State state) {
    if(state == QMediaPlayer::PlayingState) {
        ui->playButton->setEnabled(false);
        ui->pauseButton->setEnabled(true);
        ui->stopButton->setEnabled(true);
    }
    else if(state == QMediaPlayer::PausedState) {
        ui->playButton->setEnabled(true);
        ui->pauseButton->setEnabled(false);
        ui->stopButton->setEnabled(true);
    }
    else if(state == QMediaPlayer::StoppedState) {
        ui->playButton->setEnabled(true);
        ui->pauseButton->setEnabled(false);
        ui->stopButton->setEnabled(false);
        ui->labelTime->setText("00:00:00");
        ui->statusPlay->setValue(0);
        ui->labelSong->setText("");
        ui->labelArtist->setText("");
        ui->statusPlay->setEnabled(false);
    }
}

void SoundPlayer::setPlay()
{
    if (m_player->isMetaDataAvailable()) {
        ui->labelSong->setText(m_player->metaData(QStringLiteral("Title")).toString());
        ui->labelArtist->setText(m_player->metaData(QStringLiteral("ContributingArtist")).toString());
    }
    ui->statusPlay->setEnabled(true);
    m_player->play();
}

void SoundPlayer::setPause()
{
    m_player->pause();
}

void SoundPlayer::setStop()
{
    m_player->stop();
}

void SoundPlayer::setVolume(int position)
{
    m_player->setVolume(position);
}

void SoundPlayer::setPosition(int position)
{
    if (ui->statusPlay->maximum() != m_player->duration())
       ui->statusPlay->setMaximum(static_cast<int>(m_player->duration()));
    ui->statusPlay->setValue(position);

    int sececonds = (position / 1000) % 60;
    int minute = (position / 60000) % 60;
    int hours = (position / 3600000) % 24;
    QTime time{hours, minute, sececonds};
    ui->labelTime->setText(time.toString());
}

void SoundPlayer::setMovedPosition(int position)
{
    m_player->setPosition(position);
}
