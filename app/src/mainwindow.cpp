#include "settings.h"
#include "app.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "loggingcategories.h"
#include "dialogpreferences.h"
#include "dialoginfo.h"
#include "stardelegate.h"
#include "playerqss.h"
#include "nextup.h"
#include "singletonbase.h"
#include "songmodel.h"
#include <QAbstractItemView>

#include <QItemSelectionModel>
#include <QFileDialog>
#include <algorithm>
#include <QtWidgets>
#include <QSqlQueryModel>
#include <QSqlRelationalTableModel>
#include <QSqlRecord>
#include <QMouseEvent>
#include <QSqlQuery>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    readSettings();
    m_settings = new Settings();
    SingletonBase::getInstance();

//    m_base = new SqlBase();
    setupMusicTableModel();
    setupPlayListTableModel();

    m_player = new SoundPlayer(ui, this);
    m_player->setPlaylist(m_SQL_model);

//    connect(this, &MainWindow::editTagsCompleted, m_base, &SqlBase::updateTableRow);  // edit tags from Info
    connect(this, &MainWindow::editTagsCompleted, &SingletonBase::getInstance(), &SingletonBase::updateTableRow);  // edit tags from Info

    QImage def_cover(":/def_cover_black.jpg"); // default cover in player
    QPixmap pix(QPixmap::fromImage(def_cover));
    ui->cover_label->setPixmap(pix);

    ui->listPlaylist->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->listPlaylist, SIGNAL(customContextMenuRequested(const QPoint &)), this,
            SLOT(onPlayListContextMenu(const QPoint &)));

//    m_searcher = new Searcher{ui->search_line, ui->filterBox, &m_music_list};
    ui->statusbar->hide();
    ui->mainMusicTable->setContextMenuPolicy(Qt::CustomContextMenu);
//    connect(ui->filterBox, SIGNAL(activated(int)), this, SLOT(on_search_line_editingFinished()));
    // context menu for music table
    m_searcher = new Searcher(ui->search_line, ui->filterBox, m_SQL_model);
    connect(ui->mainMusicTable, SIGNAL(customContextMenuRequested(const QPoint &)), this,
            SLOT(onMusicTableContextMenu(const QPoint &)));

    connect(ui->mainMusicTable, &QAbstractItemView::clicked, this, &MainWindow::currentMusicTableIndex);
    connect(ui->listPlaylist, &QAbstractItemView::clicked, this, &MainWindow::currentPlayListIndex);
    connect(m_SQL_model, &QSqlTableModel::dataChanged, m_player, &SoundPlayer::modelChanged);
    connect(m_SQL_model, &QSqlTableModel::rowsMoved, m_player, &SoundPlayer::modelChanged);
    connect(m_SQL_model, &QSqlTableModel::layoutChanged, m_player, &SoundPlayer::modelChanged);
    connect(m_SQL_model, &QSqlTableModel::modelReset, m_player, &SoundPlayer::modelChanged);
    connect(ui->search_line, &QLineEdit::textChanged, m_searcher, &Searcher::search);
    connect(ui->filterBox, QOverload<int>::of(&QComboBox::currentIndexChanged), m_searcher, &Searcher::search);

//    connect(m_player, &SoundPlayer::playlistImported, [=](Playlist pl){ m_base->importPlayList(pl); });
    connect(m_player, &SoundPlayer::playlistImported, [=](Playlist pl){ SingletonBase::getInstance().importPlayList(pl); });

//    connect(m_player, &SoundPlayer::playlistImported, m_base, &SqlBase::importPlayList);
    connect(m_player, &SoundPlayer::playlistImported, &SingletonBase::getInstance(), &SingletonBase::importPlayList);

//    connect(m_base, &SqlBase::modelPlaylistSelect, m_PlayList_model, &QSqlTableModel::select);
    connect(&SingletonBase::getInstance(), &SingletonBase::modelPlaylistSelect, m_PlayList_model, &QSqlTableModel::select);

//    connect(m_base, &SqlBase::modelMusicSelect, m_SQL_model, &QSqlTableModel::select);
    connect(&SingletonBase::getInstance(), &SingletonBase::modelMusicSelect, m_SQL_model, &QSqlTableModel::select);

    connect(m_settings->getTimer(), &QTimer::timeout, this, &MainWindow::on_actionQuit_triggered);
    connect(m_SQL_model, &QSqlTableModel::beforeUpdate,this, &MainWindow::on_editTableModel_clicked);
    init_systemTrayIcon();
}

void MainWindow::setupMusicTableModel() {
    m_SQL_model = new QSqlTableModel(this);
//    m_SQL_model = new SongModel(this);
    SingletonBase::getInstance();

    m_SQL_model->setTable("SONGS");
    m_SQL_model->setEditStrategy(QSqlTableModel::OnFieldChange);
    m_SQL_model->select();

    m_star_delegate = new StarDelegate(ui->mainMusicTable);
    ui->mainMusicTable->setItemDelegateForColumn(4, m_star_delegate);
    ui->mainMusicTable->setEditTriggers(
//            QAbstractItemView::DoubleClicked |
            QAbstractItemView::SelectedClicked);
    ui->mainMusicTable->setSelectionBehavior(QAbstractItemView::SelectRows);

    ui->mainMusicTable->setModel(m_SQL_model);
    ui->mainMusicTable->hideColumn(0); // song_id
    ui->mainMusicTable->hideColumn(11);  // path
    ui->mainMusicTable->hideColumn(12);  // count
    ui->mainMusicTable->hideColumn(13);  // cover
    ui->mainMusicTable->hideColumn(14);  // cover
    ui->mainMusicTable->hideColumn(15);  // cover
    ui->mainMusicTable->hideColumn(16);  // cover
    ui->mainMusicTable->hideColumn(17);  // cover
    ui->mainMusicTable->resizeColumnsToContents();
}

void MainWindow::setupPlayListTableModel() {

    m_PlayList_model = new QSqlTableModel(this);
    m_PlayList_model->setTable("LIST_PLAYLISTS");
    m_PlayList_model->select();
    ui->listPlaylist->setModel(m_PlayList_model);
    ui->listPlaylist->setModelColumn(1);
    ui->listPlaylist->setEditTriggers(
//            QAbstractItemView::DoubleClicked |
    QAbstractItemView::SelectedClicked);
}


void MainWindow::onMusicTableContextMenu(const QPoint &point) {
    QMenu contextMenu(tr("Music table context menu"), this);

//    auto fullFileName = dynamic_cast<QFileSystemModel *>(ui->treeView->model())->filePath(ui->treeView->indexAt(point));

    QAction action_add_to_playlist("Add to Playlist", this);
    connect(&action_add_to_playlist, &QAction::triggered, this, &MainWindow::on_actionAddtoPlaylist_triggered);
    contextMenu.addAction(&action_add_to_playlist);

    QAction action_play_next("Play Next", this);
    connect(&action_play_next, &QAction::triggered, this, &MainWindow::playNext);
    contextMenu.addAction(&action_play_next);

    QAction action_song_info("Song Info", this);
    connect(&action_song_info, &QAction::triggered, this, &MainWindow::on_actionInfo_triggered);
    contextMenu.addAction(&action_song_info);

    QAction action_show_in_finder("Show in Finder", this);
    connect(&action_show_in_finder, &QAction::triggered, this, &MainWindow::on_action_show_in_finder_triggered);
    contextMenu.addAction(&action_show_in_finder);

    QAction action_del_from_library("Delete from Library", this);
    connect(&action_del_from_library, &QAction::triggered, this, &MainWindow::on_actionDelete_from_Library_triggered);
    contextMenu.addAction(&action_del_from_library);
    contextMenu.exec(ui->mainMusicTable->viewport()->mapToGlobal(point));
}

#include <QT>
void MainWindow::onPlayListContextMenu(const QPoint &point)
{
    currentPlayListIndex(ui->listPlaylist->currentIndex());
    QMenu contextMenu(tr("SideBar context menu"), this);

    QAction action_new("New Playlist", this);
    connect(&action_new, &QAction::triggered, this, &MainWindow::on_actionPlaylist_triggered);
    contextMenu.addAction(&action_new);

    QAction action_play("Play Playlist", this);
    connect(&action_play, &QAction::triggered, this, &MainWindow::on_actionPlayPlaylist_triggered);
    contextMenu.addAction(&action_play);

    QAction action_import_plst("Import Playlist", this);
    connect(&action_import_plst, &QAction::triggered, this, &MainWindow::on_actionImportPlaylist_triggered);
    contextMenu.addAction(&action_import_plst);

    QAction action_export_plst("Export Playlist", this);
    connect(&action_export_plst, &QAction::triggered, this, &MainWindow::on_actionExportPlaylist_triggered);
    contextMenu.addAction(&action_export_plst);

    QAction action_delete("Delete Playlist", this);
    connect(&action_delete, &QAction::triggered, this, &MainWindow::on_actionDeletePlaylist_triggered);
    contextMenu.addAction(&action_delete);
    contextMenu.exec(ui->listPlaylist->viewport()->mapToGlobal(point));
}

MainWindow::~MainWindow()
{
    qDebug(logDebug()) << "~MainWindow";
    delete m_searcher;
    delete m_SQL_model;
    delete m_PlayList_model;
    delete m_star_delegate;
    delete m_contextMenu;
    delete m_settings;
    delete nextUp;
    delete ui;
    SingletonBase::getInstance().closeDataBase();

}

void MainWindow::on_mainMusicTable_doubleClicked(const QModelIndex &index)  // player
{
    setMusicPlay(index.row());
    loadCoverImage(index);
}

void MainWindow::setMusicPlay(QString soundPath)
{
    m_player->setSound(soundPath);
}

void MainWindow::setMusicPlay(int index)
{
    m_player->setSound(index, qobject_cast<QSqlQueryModel *>(ui->mainMusicTable->model()));
}

void MainWindow::on_playButton_clicked()
{
    m_player->setPlay();
}

void MainWindow::on_pauseButton_clicked()
{
    m_player->previous();
//    m_player->setPause();
}

void MainWindow::on_stopButton_clicked()
{
    m_player->next();
//    m_player->setStop();
}

void MainWindow::on_statusPlay_valueChanged(int value)
{
    m_player->setMovedPosition(value);
}

void MainWindow::on_statusVolume_valueChanged(int value)
{
    m_player->setVolume(value);
}



void MainWindow::readSettings() {
    QSettings *settings = App::get_app()->app_settings();

//    QString theme = settings->value("theme").toString();
    const QByteArray geometry = settings->value("geometry", QByteArray()).toByteArray();
    if (geometry.isEmpty()) {
        const QRect availableGeometry = screen()->availableGeometry();
        resize(availableGeometry.width() / 3, availableGeometry.height() / 2);
        move((availableGeometry.width() - width()) / 2,
             (availableGeometry.height() - height()) / 2);
    } else {
        restoreGeometry(geometry);
    }
}

void MainWindow::writeSettings() {
    qInfo(logInfo()) << "write settings before quit";

    QSettings *settings = App::get_app()->app_settings();
    settings->setValue("geometry", saveGeometry());
}


void MainWindow::on_actionQuit_triggered()
{
    writeSettings();

    qInfo(logInfo()) << QString("Quit uamp\n");
    App::quit();
}

void MainWindow::on_actionPreferences_triggered()
{
    qDebug(logDebug()) << "on_action Preferences_triggered";

    DialogPreferences *window_settings = new DialogPreferences(m_settings->get_current_settings(), 0);
    window_settings->setModal(true);

//    QObject::connect(window_settings, &DialogSettings::SavedSettings, this, &MainWindow::applySettings);
    if (window_settings->exec() == QDialog::Accepted) {
        qInfo(logInfo()) << "exec QDilaog";
        QMap<QString, QString> new_settings;
        window_settings->get_dialog_options(new_settings);  // get settings from QDialog
//        m_settings->set_settings(new_settings);
    }
}

void MainWindow::on_actionInfo_triggered()
{
  qInfo(logInfo()) << "on_actionInfo_triggered";
    if (!m_table_index.isValid()) {
        qDebug(logDebug()) << "on_actionInfo_triggered return";
        return;
    }

    if (auto row = m_SQL_model->record(m_table_index.row()); row.isEmpty()) {
        qDebug(logDebug()) << " record from m_tableIndex empty";
        return;
    }

    Music current_song = m_player->getMusicfromTable(m_table_index);
    if (current_song.m_path.toStdString().empty()) {
      return;
    }
    DialogInfo window_songInfo = DialogInfo(current_song, 0);
//  songInfo->setWindowFlags(Qt::CustomizeWindowHint);
    window_songInfo.setModal(true);
    if (window_songInfo.exec() == QDialog::Accepted) {
      qInfo(logInfo()) << "ok DialogInfo";
        window_songInfo.get_tag_changes(new_song_info);  // get settings from QDialog
      if (!(current_song == new_song_info)) {
          if (!(TagFunctions::modify_tags(new_song_info))) {
              qInfo(logInfo()) << new_song_info.m_path << " is not writable";
          } else {
              emit editTagsCompleted(m_table_index, new_song_info);
              qInfo(logInfo()) << new_song_info.m_name << " info has been changed!!!";
          }
      }
  }
  else
    qInfo(logInfo()) << "cancel DialogInfo";
}

void MainWindow::on_editTableModel_clicked(int, QSqlRecord &) {
    Music current_song;

    current_song.m_title = m_SQL_model->record(m_table_index.row()).value("Title").toString();
    current_song.m_artist = m_SQL_model->record(m_table_index.row()).value("Artist").toString();
    current_song.m_genre = m_SQL_model->record(m_table_index.row()).value("Genre").toString();
    current_song.m_album = m_SQL_model->record(m_table_index.row()).value("Album").toString();
    current_song.m_year = m_SQL_model->record(m_table_index.row()).value("Year").toString();
    current_song.m_track = m_SQL_model->record(m_table_index.row()).value("Track").toString();
    current_song.m_comment = m_SQL_model->record(m_table_index.row()).value("Comment").toString();
    current_song.m_name = m_SQL_model->record(m_table_index.row()).value("Name").toString();
    current_song.m_path = m_SQL_model->record(m_table_index.row()).value("Path").toString();

    if (!(TagFunctions::modify_tags(current_song))) {
        qInfo(logInfo()) << current_song.m_path << " is not writable";
    } else {
//        emit editTagsCompleted(m_table_index, new_song_info);
        qInfo(logInfo()) << new_song_info.m_name << " info has been changed!!!";
    }

}


Music MainWindow::getMusicfromTable() {
    Music current_song;
    current_song.m_title = m_SQL_model->record(m_table_index.row()).value("Title").toString();
    current_song.m_time = m_SQL_model->record(m_table_index.row()).value("Time").toString();
    current_song.m_artist = m_SQL_model->record(m_table_index.row()).value("Artist").toString();
    current_song.m_rate = m_SQL_model->record(m_table_index.row()).value("Rating").toString();
    current_song.m_genre = m_SQL_model->record(m_table_index.row()).value("Genre").toString();
    current_song.m_album = m_SQL_model->record(m_table_index.row()).value("Album").toString();
    current_song.m_year = m_SQL_model->record(m_table_index.row()).value("Year").toString();
    current_song.m_track = m_SQL_model->record(m_table_index.row()).value("Track").toString();
    current_song.m_comment = m_SQL_model->record(m_table_index.row()).value("Comment").toString();
    current_song.m_name = m_SQL_model->record(m_table_index.row()).value("Name").toString();
    current_song.m_path = m_SQL_model->record(m_table_index.row()).value("Path").toString();
    current_song.m_lyrics = m_SQL_model->record(m_table_index.row()).value("Lyrics").toString();
    current_song.m_cover = m_SQL_model->record(m_table_index.row()).value("Cover").toByteArray();
    current_song.m_count = m_SQL_model->record(m_table_index.row()).value("Count").toInt();
    return current_song;
}


void MainWindow::on_actionDelete_from_Library_triggered()
{
    qInfo(logInfo()) << " on_actionDelete_from_Library_triggered";
    if (!m_table_index.isValid()) {
        qDebug(logDebug()) << "on_actionDelete_from_Library_triggered return";
        return;
    }
    m_SQL_model->removeRows(m_table_index.row(), 1);
    m_SQL_model->select();
}

void MainWindow::on_actionAdd_to_Library_triggered()  // add folders
{
    QString f_name = QFileDialog::getExistingDirectory(this, "Add media", "");
//    m_base->AddtoLibrary(f_name);
    SingletonBase::getInstance().AddtoLibrary(f_name);
    m_SQL_model->select();

    emit m_SQL_model->layoutChanged();
    emit m_SQL_model->sort(0, Qt::AscendingOrder);
    m_SQL_model->select();

//    m_player->setPlaylist(m_library->dataPlaylist()); !!!!
//    cout << "===================Setting==================" << endl;
//    for (int i = 0; i < m_library->dataPlaylist().size(); ++i )
//        cout << m_library->dataPlaylist()[i].m_path.toStdString() << endl;
//    cout << "============================================" << endl;
    ui->mainMusicTable->resizeColumnsToContents();
}

void MainWindow::loadCoverImage(const QModelIndex &index) {
    QByteArray cover = m_SQL_model->record(index.row()).value("Cover").toByteArray();

    QImage coverQImg(":/def_cover_black.jpg");
    QPixmap outPixmap = QPixmap();

    if (cover == nullptr) {
        outPixmap = (QPixmap::fromImage(coverQImg));
    }
    else {
        outPixmap.loadFromData(cover);
    }
    ui->cover_label->setPixmap(outPixmap);
}

void MainWindow::currentMusicTableIndex(const QModelIndex &index) {
    m_table_index = index;
}

void MainWindow::currentPlayListIndex(const QModelIndex &index) {
  qInfo(logInfo()) << "MainWindow::currentPlayListIndex";

  m_playList_index = index;
  auto cur_playlist = m_PlayList_model->record(m_playList_index.row()).value("Name").toString();
  qDebug(logDebug()) << "cur_playlist =" << cur_playlist;
  QSqlQuery query;
  QSqlQueryModel *model = new QSqlQueryModel;
  int PLAY_LISTS_R;

  query.prepare("SELECT PLAY_LISTS_ID FROM LIST_PLAYLISTS WHERE Name = ?");
  query.addBindValue(cur_playlist);
  query.exec();
  query.next();
  PLAY_LISTS_R =  query.value(0).toInt();
  qDebug(logDebug()) << "PLAY_LISTS_R = " << PLAY_LISTS_R;
    query.prepare("SELECT * "
                  "FROM SONGS INNER JOIN PLAYLIST ON SONGS.SONG_ID = PLAYLIST.SONG_R "
                  "WHERE PLAYLIST.PLAYLIST_R = ?");
  query.addBindValue(PLAY_LISTS_R);
  query.exec();
  model->setQuery(query);
  ui->mainMusicTable->setModel(model);
/*
  QSqlRelationalTableModel *m_SONGS_2 = new  QSqlRelationalTableModel(this);
  m_SONGS_2->setTable("SONGS");
  m_SONGS_2->setRelation(0, QSqlRelation("PLAYLIST", "SONG_ID", "SONG_R"));
  m_SONGS_2->select(); // Делаем выборку данных из таблицы
  ui->mainMusicTable->setModel(m_SONGS_2);
  m_SONGS_2->select(); // Делаем выборку данных из таблицы

  */
}

void MainWindow::on_actionPlaylist_triggered()
{
    bool ok;
    QString new_playlist_name = QInputDialog::getText(this,"New PlayList",
                                                      tr("Enter the name for new PlayList"),
                                                      QLineEdit::Normal,
                                                      "",
                                                      &ok);
    if (ok) {
        qInfo(logInfo()) << "new playlist " << new_playlist_name;
//        m_base->AddNewPlaylist(new_playlist_name);
        SingletonBase::getInstance().AddNewPlaylist(new_playlist_name);
        m_PlayList_model->select();
    } else {
        qInfo(logInfo()) << "on_actionPlaylist_triggered canceled";
    }
}

void MainWindow::on_actionImportPlaylist_triggered()
{
    QString import_playlist_name = QFileDialog::getOpenFileName(this);
    if (!import_playlist_name.isEmpty()) {
        qInfo(logInfo()) << "on_actionImportPlaylist_triggered import_playlist_name =" << import_playlist_name;
        m_player->importPlaylist(import_playlist_name);
        m_PlayList_model->select();
    } else {
      qInfo(logInfo()) << "on_actionImportPlaylist_triggered import_playlist_name  canceled";
    }
}

void MainWindow::on_actionExportPlaylist_triggered() {
    qInfo(logInfo()) << "on_actionExportPlaylist_triggered";

//    if (!ui->listPlaylist->hasFocus()) {
//        qInfo(logInfo()) << "on_actionExportPlaylist_triggered  widget no focus";
//        return;
//    }

    QFileDialog dialog(0);
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setAcceptMode(QFileDialog::AcceptSave);

    if (dialog.exec() != QDialog::Accepted)
        return ;
    QString playlist_name = m_PlayList_model->record(m_playList_index.row()).value("Name").toString();

    qInfo(logInfo()) << "playlist_name "  << playlist_name;

//    Playlist current = m_base->ExportPlaylist(playlist_name);
    Playlist current = SingletonBase::getInstance().ExportPlaylist(playlist_name);

    m_player->exportPlaylist(current, dialog.selectedFiles().first());
}

void MainWindow::on_actionDeletePlaylist_triggered() {
    QString name = m_PlayList_model->record(m_playList_index.row()).value("Name").toString();
//    m_base->DeletePlaylist(name);
    SingletonBase::getInstance().DeletePlaylist(name);
    m_PlayList_model->select();
}

void MainWindow::playNext() {
//    Music song = getMusicfromTable();
    m_player->playNext(m_player->getMusicfromTable(m_table_index));
}

void MainWindow::on_actionFast_forward_triggered()
{
    m_player->fastForward();
}

void MainWindow::on_actionRewind_triggered()
{
    m_player->rewind();
}

void MainWindow::on_actionAddtoPlaylist_triggered() {
    auto current_song_path = m_SQL_model->record(m_table_index.row()).value("Path").toString();
    auto cur_playlist = m_PlayList_model->record(m_playList_index.row()).value("Name").toString();
//    m_base->AddtoPlaylist(current_song_path, cur_playlist);
    SingletonBase::getInstance().AddtoPlaylist(current_song_path, cur_playlist);
}

void MainWindow::on_search_line_editingFinished()
{
    m_searcher->search();
}

void MainWindow::on_songs_clicked()
{
//  QSqlRelationalTableModel *m_PlayList_model_2 = new  QSqlRelationalTableModel(this);
//  m_PlayList_model_2->setTable("SONGS");
//  m_PlayList_model_2->setRelation(0, QSqlRelation("PLAYLIST", "SONG_R", "SONG_ID"));
//  ui->mainMusicTable->setModel(m_PlayList_model_2);
//  m_PlayList_model_2->select(); // Делаем выборку данных из таблицы

    ui->mainMusicTable->setModel(m_SQL_model);
    ui->mainMusicTable->hideColumn(0);
    ui->mainMusicTable->hideColumn(12);
    m_SQL_model->select();
}

void MainWindow::on_modeButton_clicked()
{
    static QString tmp = "L";
    if (tmp == "L") {
        tmp = "S";
        ui->modeButton->setStyleSheet(RandomStyle());
        m_systemTray->contextMenu()->actions()[3]->setIcon(QIcon(":/image/image/image/play-random.png"));
    }
    else if (tmp == "S") {
        tmp = "C";
        ui->modeButton->setStyleSheet(LoopOneStyle());
        m_systemTray->contextMenu()->actions()[3]->setIcon(QIcon(":/image/image/image/loop-one.png"));
    }
    else if (tmp == "C") {
        tmp = "L";
        ui->modeButton->setStyleSheet(LoopStyle());
        m_systemTray->contextMenu()->actions()[3]->setIcon(QIcon(":/image/image/image/loop.png"));

    }
    m_player->changeMode();
}

void MainWindow::on_upNextButton_clicked()
{
    if (!nextUp)
        delete nextUp;
    nextUp = new NextUp(this, m_player);
    nextUp->setGeometry(this->x() + ui->upNextButton->x() - 291 / 2, this->y() + ui->upNextButton->y() + 60, 291, 441);
    nextUp->setWindowFlags(Qt::Popup);
    nextUp->show();
}

#include <QModelIndex>

void MainWindow::on_actionPlayPlaylist_triggered() {
    on_mainMusicTable_doubleClicked(ui->mainMusicTable->model()->index(0, 0));
}

void MainWindow::on_actionAdd_Song_to_Library_triggered() {
    qDebug(logDebug) << "MainWindow::on_actionAdd_Song_to_Library_triggered";
    QStringList files = QFileDialog::getOpenFileNames(this, "Add song", "/home",
                                                  tr("Audio (*.mp3 *.flac *.wav *.m4a *.aif)"));

//    m_base->AddtoLibrary(files);
    SingletonBase::getInstance().AddtoLibrary(files);
    m_SQL_model->select();

    emit m_SQL_model->layoutChanged();
    emit m_SQL_model->sort(0, Qt::AscendingOrder);
    m_SQL_model->select();
    ui->mainMusicTable->resizeColumnsToContents();
}

void MainWindow::on_action_show_in_finder_triggered() {
    qDebug (logDebug()) << "on_action_show_in_finder_triggered";
    auto current_song_path = m_SQL_model->record(m_table_index.row()).value("Path").toString();

    QStringList scriptArgs;
    scriptArgs << QLatin1String("-e")
               << QString::fromLatin1("tell application \"Finder\" to reveal POSIX file \"%1\"")
                       .arg(current_song_path);
    QProcess::execute(QLatin1String("/usr/bin/osascript"), scriptArgs);
    scriptArgs.clear();
    scriptArgs << QLatin1String("-e")
               << QLatin1String("tell application \"Finder\" to activate");
    QProcess::execute("/usr/bin/osascript", scriptArgs);
}



void MainWindow::on_actionPlay_triggered()
{
    on_playButton_clicked();
}

void MainWindow::on_actionNext_triggered()
{
    on_stopButton_clicked();
}

void MainWindow::on_actionPrevious_triggered()
{
    on_pauseButton_clicked();
}

void MainWindow::on_actionShuffle_triggered()
{
    on_modeButton_clicked();
}

void MainWindow::on_actionIncrease_Volume_triggered()
{
    int vol = m_player->volume();
    m_player->setVolume(vol + 5);
}

void MainWindow::on_actionDecrease_Volume_triggered()
{
    int vol = m_player->volume();
    m_player->setVolume(vol - 5);
}

void MainWindow::systemTrayIcon_activated(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::DoubleClick) {
        if (isHidden()) {
            show();
        }
        else {
            hide();
        }
    }
}

void MainWindow::init_systemTrayIcon()
{
    m_systemTray = new QSystemTrayIcon(this);
    m_systemTray->setIcon(QIcon(":/image/image/image/systemTrayIcon.png"));
    m_systemTray->setToolTip("Uamp");
    connect(m_systemTray, &QSystemTrayIcon::activated, this, &MainWindow::systemTrayIcon_activated);

    QAction *action_systemTray_pre = new QAction(QIcon(":/image/image/image/pre.png"), "Previous");
    connect(action_systemTray_pre, &QAction::triggered, this, &MainWindow::on_actionPrevious_triggered);
    QAction *action_systemTray_play = new QAction(QIcon(":/image/image/image/pase.png"), "Play");
    connect(action_systemTray_play, &QAction::triggered, this, &MainWindow::on_playButton_clicked);
    QAction *action_systemTray_next = new QAction(QIcon(":/image/image/image/next.png"), "Next");
    connect(action_systemTray_next, &QAction::triggered, this, &MainWindow::on_actionNext_triggered);
    QAction *action_systemTray_playmode = new QAction(QIcon(":/image/image/image/loop.png"), "Loop Mode");
    connect(action_systemTray_playmode, &QAction::triggered, this, &MainWindow::on_modeButton_clicked);
    QAction *action_systemTray_quit = new QAction(QIcon(":/image/image/image/exit.png"), "Exit");
    connect(action_systemTray_quit, &QAction::triggered, this, &MainWindow::on_actionQuit_triggered);

    m_contextMenu = new QMenu(this);
    m_contextMenu->addAction(action_systemTray_pre);
    m_contextMenu->addAction(action_systemTray_play);
    m_contextMenu->addAction(action_systemTray_next);
    m_contextMenu->addAction(action_systemTray_playmode);
    m_contextMenu->addAction(action_systemTray_quit);
    m_contextMenu->actions();
    m_systemTray->setContextMenu(m_contextMenu);
    m_systemTray->show();
}

void MainWindow::on_actionStart_triggered() {
    qInfo(logInfo()) << "MainWindow::on_actionStart_triggered";
    bool ok;
    int minutes = QInputDialog::getInt(this, tr("ShutDown"),
                                       tr("Set minutes:"), 0, 0, 100, 1, &ok);
    if (ok) {
        m_settings->setTimer(minutes * 60000);
        qInfo(logInfo()) << "time to shutDown = " << minutes;
    } else {
        qInfo(logInfo()) << "on_actionShutDown_triggered canceled";
    }
}

void MainWindow::on_actionStopShutdown_triggered() {
    qInfo(logInfo()) << "MainWindow::on_actionStopShutdown_triggered";
    m_settings->getTimer()->stop();
}

void MainWindow::on_recentlyadded_clicked()
{
    qDebug(logDebug()) << "on_recentlyadded_clicked";
    QSqlQuery query;
    QSqlQueryModel *model_recently = new QSqlQueryModel;
  query.prepare("SELECT * FROM SONGS ORDER BY DateTime DESC LIMIT 20");
  query.exec();

//  model_recently->sort(15, Qt::AscendingOrder);
  model_recently->setQuery(query);
  ui->mainMusicTable->setModel(model_recently);
}

void MainWindow::on_artists_clicked()
{
    qDebug(logDebug()) << "on_artists_clicked";
  QSqlQuery query;
  QSqlQueryModel *model_artists = new QSqlQueryModel;
  query.prepare("SELECT DISTINCT Artist FROM SONGS");
  query.exec();
  ui->mainMusicTable->showColumn(0);
  model_artists->setQuery(query);
  ui->mainMusicTable->setModel(model_artists);
}

void MainWindow::on_albums_clicked()
{
    qDebug(logDebug()) << "on_albums_clicked";
  QSqlQuery query;
  QSqlQueryModel *model_Album = new QSqlQueryModel;
  query.prepare("SELECT DISTINCT Album FROM SONGS");
  query.exec();
  ui->mainMusicTable->showColumn(0);
  model_Album->setQuery(query);
  ui->mainMusicTable->setModel(model_Album);
}

void MainWindow::on_genres_clicked()
{
    qDebug(logDebug()) << "on_genres_clicked";
  QSqlQuery query;
  QSqlQueryModel *model_genre = new QSqlQueryModel;
  query.prepare("SELECT DISTINCT Genre FROM SONGS");
  query.exec();
  ui->mainMusicTable->showColumn(0);
  model_genre->setQuery(query);
  ui->mainMusicTable->setModel(model_genre);
}