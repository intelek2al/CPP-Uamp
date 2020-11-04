#include "soundPlayer.h"
#include "ui_mainwindow.h"
#include <QWidget>

SoundPlayer::SoundPlayer(Ui::MainWindow *child)
{
    m_player = new QMediaPlayer();
    m_playlist = SoundPlayer::playlist();
    m_playlist->setPlaybackMode(QMediaPlaylist::PlaybackMode::Loop);
    m_player->setPlaylist(m_playlist);
    ui = child;
    connect(m_player, &QMediaPlayer::positionChanged, this, &SoundPlayer::setPosition);
    connect(m_player, &QMediaPlayer::stateChanged, this, &SoundPlayer::stateCheck);
//    connect(m_player, QOverload<bool>::of(&QMediaObject::availabilityChanged) &SoundPlayer::metaData);
    connect(m_player, &QMediaPlayer::metaDataAvailableChanged, this, &SoundPlayer::metaData);
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
    if (!isPlaylistExist)
        return;
//    m_player->setMedia(QUrl::fromLocalFile(path));
    if (m_player->isMetaDataAvailable())
        ui->statusbar->showMessage(m_player->currentMedia().playlist()->objectName(), 2000);
    else {
        while (!m_player->isMetaDataAvailable()) {
            QCoreApplication::processEvents();
        }
    }
    QStringList list = m_player->availableMetaData();

    this->setPlay();
    ui->playButton->click();
}

void SoundPlayer::setSound(int index) {
    if (!isPlaylistExist)
        return;

    m_list.setStartSong(index);
//    QStringList list = m_player->availableMetaData();
    ui->playButton->click();
}

void SoundPlayer::stateCheck(QMediaPlayer::State state) {
//    if(state == QMediaPlayer::PlayingState) {
//        ui->playButton->setEnabled(false);
//        ui->pauseButton->setEnabled(true);
//        ui->stopButton->setEnabled(true);
//    }
//    else if(state == QMediaPlayer::PausedState) {
//        ui->playButton->setEnabled(true);
//        ui->pauseButton->setEnabled(false);
//        ui->stopButton->setEnabled(true);
//    }
//    else if(state == QMediaPlayer::StoppedState) {
//        ui->playButton->setEnabled(true);
//        ui->pauseButton->setEnabled(false);
//        ui->stopButton->setEnabled(false);
//        ui->labelTime->setText("00:00:00");
//        ui->statusPlay->setValue(0);
//        ui->labelSong->setText("");
//        ui->labelArtist->setText("");
//        ui->statusPlay->setEnabled(false);
//    }
}
#include <iostream>
void SoundPlayer::setPlay()
{
    QWidget tmp;

    metaData(m_player->isMetaDataAvailable());
    if (m_player->state() == QMediaPlayer::State::PausedState || m_player->state() == QMediaPlayer::State::StoppedState) {
        m_player->play();
        ui->playButton->setIcon(tmp.style()->standardIcon(QStyle::SP_MediaPause));
    }
    else if(m_player->state() == QMediaPlayer::State::PlayingState) {
        m_player->pause();
        ui->playButton->setIcon(tmp.style()->standardIcon(QStyle::SP_MediaPlay));
    }
    ui->statusPlay->setEnabled(true);
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

    int seconds = (position / 1000) % 60;
    int minute = (position / 60000) % 60;
    int hours = (position / 3600000) % 24;
    QTime time{hours, minute, seconds};
    ui->labelTime->setText(time.toString());
}

void SoundPlayer::fastForward() {
    int pos = m_player->position();
    pos += 10 * 1000;
    m_player->setPosition(pos);
}

void SoundPlayer::rewind()
{
    int pos = m_player->position();
    pos += -10 * 1000;
    m_player->setPosition(pos);
}
void SoundPlayer::setMovedPosition(int position)
{
    m_player->setPosition(position);
}

QMediaPlaylist *SoundPlayer::m_playlist = nullptr;

QMediaPlaylist *SoundPlayer::playlist() {
    if (m_playlist == nullptr)
        m_playlist = new QMediaPlaylist();
    return m_playlist;
}

void SoundPlayer::setPlaylist(const Playlist &playlist) {
    m_list.setPlaylist(playlist);
    isPlaylistExist = true;
}

void SoundPlayer::setPlaylist(QSqlTableModel *model) {
    m_model = model;
    Playlist playlist = handlerPlaylist(m_model);
    setPlaylist(playlist);
}

void SoundPlayer::next() {
    m_list.next();
    setPlay();
    system("clear");
    auto test_up = m_list.upNext();
    auto test_hs = m_list.history();
    std::cout << " = = = = = = = = =  History  = = = = = = = = = \n";
    for (int i = test_hs.size() - 1; i >= 0; --i) {
        std::cout << test_hs[i].getStr().toStdString() << std::endl;
    }
    std::cout << " = = = = = = = = =  Up Next  = = = = = = = = = \n";
    for (int i = 0; i < test_up.size(); i++) {
        std::cout << test_up[i].getStr().toStdString() << std::endl;
    }

}

void SoundPlayer::previous() {
    m_list.previous();
    setPlay();
    system("clear");
    auto test = m_list.upNext();
    auto test_up = m_list.upNext();
    auto test_hs = m_list.history();
    std::cout << " = = = = = = = = =  History  = = = = = = = = = \n";
    for (int i = test_hs.size() - 1; i >= 0; --i) {
        std::cout << test_hs[i].getStr().toStdString() << std::endl;
    }
    std::cout << " = = = = = = = = =  Up Next  = = = = = = = = = \n";
    for (int i = 0; i < test_up.size(); i++) {
        std::cout << test_up[i].getStr().toStdString() << std::endl;
    }
}

void SoundPlayer::savePlaylist(Playlist playlist, QString path) {
    QMediaPlaylist _pl;
    for (size_t i = 0; i < playlist.size(); ++i) {
        _pl.addMedia(playlist[i].m_url);
    }
    path += "test.m3u";
    if (_pl.save(QUrl::fromLocalFile(path)))
        ui->statusbar->showMessage("saved", 1200);
    else
    ui->statusbar->showMessage("not saved: " + _pl.errorString(), 12000);
}

void SoundPlayer::playNext(const Music &song) {
    m_list.addUserNext(song);
}

void SoundPlayer::metaData(bool check) {
    std::cout << "Avalible: " << check << std::endl;
    if (check) {
        auto title = m_player->metaData(QStringLiteral("Title")).toString();
        ui->labelSong->setText(title.isEmpty() ? "Untitled" : title);
        ui->labelArtist->setText(m_player->metaData(QStringLiteral("ContributingArtist")).toString());
    }
    else {
        auto title = m_list.currentMusic().m_title;
        auto artist = m_list.currentMusic().m_artist;
        ui->labelSong->setText(title);
        ui->labelArtist->setText(artist);
    }
}

void SoundPlayer::modelChanged() {
    std::cout << "= = = = = = = Reloading = = = = = = = " << std::endl;
    Playlist playlist = handlerPlaylist(m_model);
    m_list.setChangedPlaylist(playlist);
}

Playlist SoundPlayer::handlerPlaylist(QSqlTableModel *model) {
    Playlist playlist;
    std::cout << "= = = = = = = Loading = = = = = = = " << std::endl;
    for (int i = 0; i < model->rowCount(); ++i) {
        Music current_song;
        current_song.m_title = model->record(i).value("Title").toString();
        current_song.m_time = model->record(i).value("Time").toString();
        current_song.m_artist = model->record(i).value("Artist").toString();
        current_song.m_rate = model->record(i).value("Rating").toString();
        current_song.m_genre = model->record(i).value("Genre").toString();
        current_song.m_album = model->record(i).value("Album").toString();
        current_song.m_year = model->record(i).value("Year").toString();
        current_song.m_track = model->record(i).value("Track").toString();
        current_song.m_comment = model->record(i).value("Comment").toString();
        current_song.m_name = model->record(i).value("Name").toString();
        current_song.m_path = model->record(i).value("Path").toString();
        std::cout << "  Song : " << current_song.m_title.toStdString() << std::endl;
        playlist.addMusic(current_song);
    }
    return playlist;
}
