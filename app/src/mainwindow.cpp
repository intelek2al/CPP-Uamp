#include "settings.h"
#include "app.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "loggingcategories.h"
#include "dialogpreferences.h"
#include "dialoginfo.h"
#include "stardelegate.h"
#include <QAbstractItemView>
#include <QItemSelectionModel>

#include <QFileDialog>
#include <algorithm>
#include <QtWidgets>
#include <QSqlQueryModel>
#include <QSqlRelationalTableModel>
#include <QSqlRecord>
#include <QSqlQuery>
#include "nextup.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    readSettings();

    m_settings = new Settings();
    m_base = new SqlBase();
//    m_library = new MediaLibrary(); //!!!!!!!!!!!!!
//    m_tableModel = new MusicTableModel(*m_library,  ui->mainMusicTable);

    setupMusicTableModel();
    setupPlayListTableModel();

    m_player = new SoundPlayer(ui);
//    m_player->setPlaylist(m_library->dataPlaylist());
    m_player->setPlaylist(m_SQL_model);

//    m_selection_model = new QItemSelectionModel(m_tableModel);
//    ui->mainMusicTable->setSelectionModel(m_selection_model);

    connect(this, &MainWindow::editTagsCompleted, m_base, &SqlBase::updateTableRow);  // edit tags from Info

    ui->playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    ui->pauseButton->setIcon(style()->standardIcon(QStyle::SP_MediaSkipBackward));

//    ui->stopButton->setIcon(style()->standardIcon(QStyle::SP_MediaStop));
    ui->stopButton->setIcon(style()->standardIcon(QStyle::SP_MediaSkipForward));

    // default cover in player
    QImage def_cover(":/def_cover_black.jpg");
    QPixmap pix(QPixmap::fromImage(def_cover));
    ui->cover_label->setPixmap(pix);

    // context menu for ...
    ui->sideBar->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->sideBar, SIGNAL(customContextMenuRequested(const QPoint &)), this,
            SLOT(onSideBarContextMenu(const QPoint &)));

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
    connect(m_player, &SoundPlayer::playlistImported, [=](Playlist pl){ m_base->importPlayList(pl); });
    connect(m_player, &SoundPlayer::playlistImported, m_base, &SqlBase::importPlayList);

    connect(m_base, &SqlBase::modelPlaylistSelect, m_PlayList_model, &QSqlTableModel::select);
    connect(m_base, &SqlBase::modelMusicSelect, m_SQL_model, &QSqlTableModel::select);


}


void MainWindow::setupMusicTableModel() {
    m_SQL_model = new QSqlTableModel(this);
    m_SQL_model->setTable("SONGS");
    m_SQL_model->setEditStrategy(QSqlTableModel::OnFieldChange);
    m_SQL_model->select();

    m_star_delegate = new StarDelegate(ui->mainMusicTable);
//    ui->mainMusicTable->setModel(m_tableModel);

    ui->mainMusicTable->setItemDelegateForColumn(4, m_star_delegate);
    ui->mainMusicTable->setEditTriggers(
//            QAbstractItemView::DoubleClicked |
            QAbstractItemView::SelectedClicked);

    ui->mainMusicTable->setSelectionBehavior(QAbstractItemView::SelectRows);

    ui->mainMusicTable->setModel(m_SQL_model);
    ui->mainMusicTable->hideColumn(0); // song_id
//    ui->mainMusicTable->hideColumn(11);  // path
    ui->mainMusicTable->hideColumn(12);  // cover
}

void MainWindow::setupPlayListTableModel() {
    m_PlayList_model = new QSqlTableModel;
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

    QModelIndex index = ui->mainMusicTable->indexAt(point);

//    auto fullFileName = dynamic_cast<QFileSystemModel *>(ui->treeView->model())->filePath(ui->treeView->indexAt(point));

    QAction action_add_to_playlist("Add to Playlist", this);
    connect(&action_add_to_playlist, &QAction::triggered, this, &MainWindow::on_actionAddtoPlaylist_triggered);
    contextMenu.addAction(&action_add_to_playlist);

    QAction* actionA_Setup = contextMenu.addAction( "Setup" );

    QAction action_play_next("Play Next", this);
    connect(&action_play_next, &QAction::triggered, this, &MainWindow::playNext);
    contextMenu.addAction(&action_play_next);

//    QAction action_play_later("Play Later", this);
//    connect(&action_play_layte, &QAction::triggered, this, &MainWindow::on_actionPlaylist_triggered);
//    contextMenu.addAction(&action_play_later);

    QAction action_song_info("Song Info", this);
    connect(&action_song_info, &QAction::triggered, this, &MainWindow::on_actionInfo_triggered);
    contextMenu.addAction(&action_song_info);

//    QAction action_like("Like", this);
//    connect(&action_play_layte, &QAction::triggered, this, &MainWindow::on_actionPlaylist_triggered);
//    contextMenu.addAction(&action_like);

//    QAction action_dislike("Dislike", this);
//    connect(&action_play_layte, &QAction::triggered, this, &MainWindow::on_actionPlaylist_triggered);
//    contextMenu.addAction(&action_dislike);

    QAction action_show_in_finder("Show in Finder", this);
//    connect(&action_play_layte, &QAction::triggered, this, &MainWindow::on_actionPlaylist_triggered);
    contextMenu.addAction(&action_show_in_finder);

    QAction action_del_from_library("Delete from Library", this);
    connect(&action_del_from_library, &QAction::triggered, this, &MainWindow::on_actionDelete_from_Library_triggered);
    contextMenu.addAction(&action_del_from_library);

    contextMenu.exec(mapToGlobal(point));
}


void MainWindow::onSideBarContextMenu(const QPoint &point)
{
    QMenu contextMenu(tr("SideBar context menu"), this);
//    auto fullFileName = dynamic_cast<QFileSystemModel *>(ui->treeView->model())->filePath(ui->treeView->indexAt(point));

    QAction action_new("New Playlist", this);
    connect(&action_new, &QAction::triggered, this, &MainWindow::on_actionNewPlaylist_triggered);
//    connect(ui->actionPlaylist, &QAction::triggered, this, &MainWindow::on_actionNewPlaylist_triggered);
    contextMenu.addAction(&action_new);

    QAction action_play("Play Playlist", this);
    connect(&action_play, &QAction::triggered, this, &MainWindow::on_actionPlqyPlaylist_triggered);
//    connect(ui->actionPlaylist, &QAction::triggered, this, &MainWindow::on_actionNewPlaylist_triggered);
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
    contextMenu.exec(mapToGlobal(point));
}

MainWindow::~MainWindow()
{
    delete m_searcher;
//    delete  m_library;
    delete nextUp;
    delete ui;
    system("leaks -q uamp");
}

//void MainWindow::on_mainMusicTable_clicked(const QModelIndex &index)
//{
//    m_table_index = index;
//}

void MainWindow::on_mainMusicTable_doubleClicked(const QModelIndex &index)  // player
{
//    Music current = m_library->data()[index.row()];
//    outputCurrentInfo(current, index);

//    setMusicPlay(current.m_path);
    setMusicPlay(index.row());

    loadCoverImage(index);
}

void MainWindow::setMusicPlay(QString soundPath)
{
    m_player->setSound(soundPath);
}

void MainWindow::setMusicPlay(int index)
{
    m_player->setSound(index);
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
//      Music current = m_library->data()[m_table_index.row()];
//    auto index = m_selection_model->selection().indexes();

//    if (!index.front().isValid() && (!(m_library->data().empty()))) {
    if (!m_table_index.isValid()) {
        qDebug(logDebug()) << "on_actionInfo_triggered return";
        return;
    }

    if (auto row = m_SQL_model->record(m_table_index.row()); row.isEmpty()) {
        qDebug(logDebug()) << " record from m_tableIndex empty";
        return;
    }

    Music current_song = getMusicfromTable();

//            m_library->data()[index.front().row()];

    DialogInfo window_songInfo = DialogInfo(current_song, 0);

//  songInfo->setWindowFlags(Qt::CustomizeWindowHint);
    window_songInfo.setModal(true);

    if (window_songInfo.exec() == QDialog::Accepted) {
      qInfo(logInfo()) << "ok DialogInfo";

        window_songInfo.get_tag_changes(new_song_info);  // get settings from QDialog
        qDebug(logDebug()) << "curent_music =" << new_song_info.m_name;
        qDebug(logDebug()) << "curent_music =" << new_song_info.m_title;
        qDebug(logDebug()) << "curent_music =" << new_song_info.m_artist;


      if (!(current_song == new_song_info)) {
          // save new tags;
          if (!(TagFunctions::modify_tags(new_song_info))) {
              qInfo(logInfo()) << new_song_info.m_path << " is not writable";
          } else {
              emit editTagsCompleted(m_table_index, new_song_info);
//                m_library->setData(m_table_index.row(), new_song_info);
//                m_SQL_model->setRecord();
              qInfo(logInfo()) << new_song_info.m_name << " info has been changed!!!";
          }
      }
  }
  else
    qInfo(logInfo()) << "cancel DialogInfo";
}

Music MainWindow::getMusicfromTable() {
    //    {"Title", "Time", "Artist", "Rating", "Genre", "Album", "Year", "Track", "Comment", "Name","Path"};
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
    current_song.m_cover = m_SQL_model->record(m_table_index.row()).value("Cover").toByteArray();
    return current_song;
}


void MainWindow::on_actionDelete_from_Library_triggered()
{
//    auto index = m_selection_model->selection().indexes();

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
//    m_library->add_media(f_name);

    m_base->AddtoLibrary(f_name);
    m_SQL_model->select();

    emit m_SQL_model->layoutChanged();
    emit m_SQL_model->sort(0, Qt::AscendingOrder);

    m_SQL_model->select();


//    m_player->setPlaylist(m_library->dataPlaylist()); !!!!

//    cout << "===================Setting==================" << endl;
//    for (int i = 0; i < m_library->dataPlaylist().size(); ++i )
//        cout << m_library->dataPlaylist()[i].m_path.toStdString() << endl;
//    cout << "============================================" << endl;
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
    m_playList_index = index;

//  auto current_song_path = m_SQL_model->record(m_table_index.row()).value("Path").toString();
  auto cur_playlist = m_PlayList_model->record(m_playList_index.row()).value("Name").toString();

    qDebug(logDebug()) << "click on playlists";
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

//  query.prepare("SELECT * FROM SONGS "
//                "INNER JOIN PLAYLIST ON SONGS.SONG_ID = PLAYLIST.SONG_R "
//                "INNER JOIN LIST_PLAYLISTS ON PLAYLIST.PLAYLIST_R = ?");

//    query.prepare("SELECT SONGS.SONG_ID, SONGS.Title, SONGS.Artist, SONGS.Album, SONGS.Rating, SONGS.Year, SONGS.Genre, SONGS.Time "
    query.prepare("SELECT * "
                  "FROM SONGS INNER JOIN PLAYLIST ON SONGS.SONG_ID = PLAYLIST.SONG_R "
                  "WHERE PLAYLIST.PLAYLIST_R = ?");
  query.addBindValue(PLAY_LISTS_R);
  query.exec();
  model->setQuery(query);

  ui->mainMusicTable->setModel(model);

//    QSqlRelationalTableModel *r_model = new QSqlRelationalTableModel;
//    r_model->setTable("SONGS");
//    r_model->setRelation(0, QSqlRelation("PLAYLIST_ID", "SONG_ID", "SONG_ID"));
//    ui->mainMusicTable->setModel(r_model);
    // show in table list of songs current playList
}

void MainWindow::on_actionNewPlaylist_triggered()
{
    bool ok;
    QString new_playlist_name = QInputDialog::getText(this,"New PlayList",
                                                      tr("Enter the name for new PlayList"),
                                                      QLineEdit::Normal,
                                                      "",
                                                      &ok);
    if (ok) {
        qInfo(logInfo()) << "new playlist " << new_playlist_name;
        m_base->AddNewPlaylist(new_playlist_name);
        m_PlayList_model->select();
    } else {
//            QMessageBox::warning(0, "Warning ", "You cannot delete the folder");  // show warning
        qInfo(logInfo()) << "Warning You cannot delete the folder " << new_playlist_name;
    }

}

void MainWindow::on_actionImportPlaylist_triggered() {
    bool ok;


    QString import_playlist_name = QFileDialog::getOpenFileName(this);
    if (!import_playlist_name.isEmpty()) {
        qInfo(logInfo()) << "on_actionImportPlaylist_triggered import_playlist_name =" << import_playlist_name;

        m_player->importPlaylist(import_playlist_name);
        m_PlayList_model->select();

    }
    qInfo(logInfo()) << "on_actionImportPlaylist_triggered import_playlist_name =" << import_playlist_name;




//        QString fileName = QFileDialog::getOpenFileName(this);
//        if (!fileName.isEmpty()) {
//            m_file_manager->loadFile(fileName);
//        }
//
//        QFile file(fullFileName);
//        if (!file.open(QFile::ReadOnly | QFile::Text)) {
//            QString warning = QString("Cannot read file %1:\n%2.").arg(QDir::toNativeSeparators(fullFileName),
//                                                                       file.errorString());
//            QMessageBox::warning (0, "Application", warning);  // show warning
//            qWarning(logWarning()) << warning;  // log warning
//            return;
//
//    }
//    else {
//        qInfo(logInfo()) << "on_actionImportPlaylist_triggered  canceled";
//    }
}

void MainWindow::on_actionExportPlaylist_triggered() {
    qInfo(logInfo()) << "on_actionExportPlaylist_triggered";

    QWidget *fw = qApp->focusWidget();
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

    Playlist current = m_base->ExportPlaylist(playlist_name);

    m_player->exportPlaylist(current, dialog.selectedFiles().first());
}


void MainWindow::on_actionDeletePlaylist_triggered() {
    QString name = m_PlayList_model->record(m_playList_index.row()).value("Name").toString();
    m_base->DeletePlaylist(name);
    m_PlayList_model->select();
}

void MainWindow::playNext() {
//    Music song = getMusicfromTable();
    m_player->playNext(getMusicfromTable());
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
    m_base->AddtoPlaylist(current_song_path, cur_playlist);
}



void MainWindow::on_search_line_editingFinished()
{
//    cout << "ASDASDASDASDASDASDASDASDASDASDASDASD" << endl;
//    readDir(ui->fileBrowser->currentIndex());
    m_searcher->search();
//    if (!m_tableModel)
//        delete m_tableModel;
//    m_tableModel = new MusicTableModel(ui->mainMusicTable);
//    m_tableModel->music_list_add(m_music_list);
//    ui->mainMusicTable->setModel(m_tableModel);
//    ui->mainMusicTable->viewport()->update();
}



//void MainWindow::outputCurrentInfo(const QVector<QString> &current, const QModelIndex &index)
//{
//    m_tableViewer->setNewItems(current, index);
//}


//void MainWindow::on_pushButton_clicked()
//{
//    auto newSongTag = m_tableViewer->getResult();
//
//    if (newSongTag[8].isEmpty() || newSongTag[0].isEmpty()) {
//        return;
//    }
//    if (!(modify_tags(newSongTag))) {
//        ui->statusbar->showMessage(newSongTag[8] + " is not writable", 200);
////        m_log->add_log_massage(newSongTag[8] + " is not writable");
//    }
//
//    m_music_list[m_tableViewer->getIndex().row()] = std::move(newSongTag);
//    m_tableModel->music_list_add(m_music_list);
//    ui->mainMusicTable->setModel(m_tableModel);
//    ui->mainMusicTable->viewport()->update();
//}







/*
QFileDialog* _f_dlg = new QFileDialog(this);
_f_dlg->setFileMode(QFileDialog::Directory);
_f_dlg->setOption(QFileDialog::DontUseNativeDialog, true);

// Try to select multiple files and directories at the same time in QFileDialog
QListView *l = _f_dlg->findChild<QListView*>("listView");
if (l) {
    l->setSelectionMode(QAbstractItemView::MultiSelection);
}
QTreeView *t = _f_dlg->findChild<QTreeView*>();
if (t) {
    t->setSelectionMode(QAbstractItemView::MultiSelection);
}

//    int nMode =
    if (_f_dlg->exec()) {
        QStringList _fnames = _f_dlg->selectedFiles();
        qDebug(logDebug()) << "add media " << _fnames.size();
        for (int i = 0; i < _fnames.size(); ++i) {
            m_library->add_media(_fnames.at(i));
        }


        if (!m_tableModel)
            delete m_tableModel;
        m_tableModel = new MusicTableModel(ui->mainMusicTable);
//            m_searcher->setDown();
        m_tableModel->music_list_add(m_library->data());
        ui->mainMusicTable->setModel(m_tableModel);
    }
    */

void MainWindow::on_songs_clicked()
{
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
        ui->modeButton->setText("S");
    }
    else if (tmp == "S") {
        tmp = "C";
        ui->modeButton->setText("C");
    }
    else if (tmp == "C") {
        tmp = "L";
        ui->modeButton->setText("L");
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

void MainWindow::on_actionPlqyPlaylist_triggered() {
    // play selected playlist
}



