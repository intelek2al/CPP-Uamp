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


char *toChar2(QString str)
{
    char *test = str.toUtf8().data();
    return test;
}


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    readSettings();

    m_settings = new Settings();
    m_player = new SoundPlayer(ui);
    m_library = new MediaLibrary();

    m_base = new SqlBase();

//    QSqlRelationalTableModel

    m_tableModel = new MusicTableModel(*m_library,  ui->mainMusicTable);

    QSqlTableModel m_SQL_model;

    m_SQL_model.setTable("SONGS");
    m_SQL_model.select();

//    ui->mainMusicTable->setModel(&m_SQL_model);


    m_selection_model = new QItemSelectionModel(m_tableModel);

    m_star_delegate = new StarDelegate(ui->mainMusicTable);


    ui->mainMusicTable->setModel(m_tableModel);

    ui->mainMusicTable->setItemDelegateForColumn(3, m_star_delegate);

    ui->mainMusicTable->setEditTriggers(
//            QAbstractItemView::DoubleClicked |
                                  QAbstractItemView::SelectedClicked);

    ui->mainMusicTable->setSelectionBehavior(QAbstractItemView::SelectRows);

  ui->mainMusicTable->setSelectionModel(m_selection_model);

    connect(this, &MainWindow::editTagsCompleted, m_tableModel, &MusicTableModel::saveTags);

    ui->playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    ui->pauseButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
    ui->stopButton->setIcon(style()->standardIcon(QStyle::SP_MediaStop));

    // default cover in player
    QImage def_cover(":/def_cover_color.png");
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
    connect(ui->mainMusicTable, SIGNAL(customContextMenuRequested(const QPoint &)), this,
            SLOT(onMusicTableContextMenu(const QPoint &)));

}

void MainWindow::onMusicTableContextMenu(const QPoint &point) {
    QMenu contextMenu(tr("Music table context menu"), this);

    QModelIndex index = ui->mainMusicTable->indexAt(point);

//    auto fullFileName = dynamic_cast<QFileSystemModel *>(ui->treeView->model())->filePath(ui->treeView->indexAt(point));

    QAction action_add_to_playlist("Add to Playlist", this);
//    connect(&action_play_layte, &QAction::triggered, this, &MainWindow::on_actionPlaylist_triggered);
    contextMenu.addAction(&action_add_to_playlist);

    QAction action_play_next("Play Next", this);
//    connect(&action_play_layte, &QAction::triggered, this, &MainWindow::on_actionPlaylist_triggered);
    contextMenu.addAction(&action_play_next);

//    QAction action_play_later("Play Later", this);
//    connect(&action_play_layte, &QAction::triggered, this, &MainWindow::on_actionPlaylist_triggered);
//    contextMenu.addAction(&action_play_later);

    QAction action_song_info("Song Info", this);
    connect(&action_song_info, &QAction::triggered, this, &MainWindow::on_actionInfo_triggered);
    contextMenu.addAction(&action_song_info);

    QAction action_like("Like", this);
//    connect(&action_play_layte, &QAction::triggered, this, &MainWindow::on_actionPlaylist_triggered);
    contextMenu.addAction(&action_like);

    QAction action_dislike("Dislike", this);
//    connect(&action_play_layte, &QAction::triggered, this, &MainWindow::on_actionPlaylist_triggered);
    contextMenu.addAction(&action_dislike);
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
    connect(&action_new, &QAction::triggered, this, &MainWindow::on_actionPlaylist_triggered);
    contextMenu.addAction(&action_new);

//    QAction action_rename("Rename ", this);
//    connect(&action_rename, &QAction::triggered, this, [=] () { on_action_context_file_rename(fullFileName); });
//    contextMenu.addAction(&action_rename);
//    QAction action_delete("Delete ", this);
//    connect(&action_delete, &QAction::triggered, this, [=] { on_action_context_file_delete(fullFileName) ;});
//    contextMenu.addAction(&action_delete);

    contextMenu.exec(mapToGlobal(point));
}


void MainWindow::on_actionPlaylist_triggered()
{
    bool ok;
    QString new_playlist_name = QInputDialog::getText(this,"New folder",
                                                    tr("Enter the new path for the new folder"),
                                                    QLineEdit::Normal,
                                                    "",
                                                    &ok);
    if (ok) {
            qInfo(logInfo()) << "new playlist " << new_playlist_name;
        } else {
//            QMessageBox::warning(0, "Warning ", "You cannot delete the folder");  // show warning
        qInfo(logInfo()) << "Warning You cannot delete the folder " << new_playlist_name;
    }
}


MainWindow::~MainWindow()
{
//    delete m_searcher;
    delete  m_library;
    delete ui;
    system("leaks -q uamp");
}

void MainWindow::on_mainMusicTable_clicked(const QModelIndex &index)
{
    m_table_index = index;
}

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

//void MainWindow::on_search_line_editingFinished()
//{
////    readDir(ui->fileBrowser->currentIndex());
//    auto tmp = m_searcher->search();
//    m_music_list = tmp;
//    if (!m_tableModel)
//        delete m_tableModel;
//    m_tableModel = new MusicTableModel(ui->mainMusicTable);
//    m_tableModel->music_list_add(m_music_list);
//    ui->mainMusicTable->setModel(m_tableModel);
//    ui->mainMusicTable->viewport()->update();
//}

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
    auto index = m_selection_model->selection().indexes();

    if (!index.front().isValid() && (!(m_library->data().empty()))) {
        qDebug(logDebug()) << "on_actionInfo_triggered return";
        return;
    }
    Music current = m_library->data()[index.front().row()];

    DialogInfo songInfo = DialogInfo(current, 0);
//  songInfo->setWindowFlags(Qt::CustomizeWindowHint);
    songInfo.setModal(true);

    if (songInfo.exec() == QDialog::Accepted) {
      qInfo(logInfo()) << "ok DialogInfo";

      songInfo.get_tag_changes(new_song_info);  // get settings from QDialog
      if (!(current == new_song_info)) {
          // save new tags;
          if (!(TagFunctions::modify_tags(new_song_info))) {
              qInfo(logInfo()) << new_song_info.m_path << " is not writable";
          } else {
              emit editTagsCompleted(index.front(), new_song_info);
                m_library->setData(index.front().row(), new_song_info);
              qInfo(logInfo()) << new_song_info.m_name << " info has been changed!!!";
          }
      }
  }
  else
    qInfo(logInfo()) << "cancel DialogInfo";
}


void MainWindow::on_actionDelete_from_Library_triggered()
{
    auto index = m_selection_model->selection().indexes();
    qInfo(logInfo()) << " on_actionDelete_from_Library_triggered";
//    qInfo(logInfo()) << " song =" << m_library->data()[index.front().row()][0];

    if (!index.front().isValid() && (!(m_library->data().empty()))) {
        qDebug(logDebug()) << "on_actionDelete_from_Library_triggered return";
        return;
    }
    m_tableModel->removeRows(index.front().row(), 1, index.front());
}



void MainWindow::on_actionAdd_to_Library_triggered()  // add folders
{
    QString f_name = QFileDialog::getExistingDirectory(this, "Add media", "");
    m_library->add_media(f_name);

    m_base->AddtoLibrary(f_name);

    qDebug(logDebug()) << "m_library size=" << m_library->data().size();
    emit m_tableModel->layoutChanged();
    emit m_tableModel->sort(0, Qt::AscendingOrder);

    m_player->setPlaylist(m_library->dataPlaylist());
}

void MainWindow::loadCoverImage(const QModelIndex &index) {

    std::string fileName = m_library->data()[index.row()].m_name.toStdString().data();
    std::string fileType = fileName.substr(fileName.size() - 3);

    QImage coverQImg(":/def_cover_color.png");

    if (fileType == "mp3") {
        coverQImg = TagFunctions::load_cover_image_mpeg(m_library->data()[index.row()].m_path.toStdString().data());
        ui->statusbar->showMessage(tr( " loaded"), 200);
    }
    if (fileType == "m4a") {
        coverQImg = TagFunctions::load_cover_image_m4a(m_library->data()[index.row()].m_path.toStdString().data());
        ui->statusbar->showMessage(tr( " loaded"), 200);
    }
    QPixmap pix(QPixmap::fromImage(coverQImg));
    ui->cover_label->setPixmap(pix);
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

