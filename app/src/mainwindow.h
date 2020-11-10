#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>
#include "tag_functions.h"
#include "tableviewer.h"
#include "musictablemodel.h"
#include "soundPlayer.h"
#include "searcher.h"
#include "settings.h"
#include "medialibrary.h"
#include "stardelegate.h"
#include "sqlbase.h"

#include <stdexcept>
#include <QMainWindow>
#include <QtCore>
#include <QtGui>
#include <QFileSystemModel>
#include <QDir>
#include <QTreeView>
#include <QString>
#include <QStringList>
#include <QGraphicsPixmapItem>
#include <QMediaPlayer>
#include <QtMultimedia>
#include <QMediaMetaData>
#include <QMetaDataReaderControl>
#include <QSqlTableModel>

QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE

class SoundPlayer;
class NextUp;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
//    void on_mainMusicTable_clicked(const QModelIndex &index);
//    void on_search_line_editingFinished();
    void currentMusicTableIndex(const QModelIndex &index);  // index of current sohg
    void currentPlayListIndex(const QModelIndex &index);   // index of current playList
    void on_mainMusicTable_doubleClicked(const QModelIndex &index);  // player
    void on_playButton_clicked();
    void on_pauseButton_clicked();
    void on_stopButton_clicked();
    void on_statusPlay_valueChanged(int value);
    void on_statusVolume_valueChanged(int value);
    void on_actionQuit_triggered();
    void onSideBarContextMenu(const QPoint &point);
    void onMusicTableContextMenu(const QPoint &point);
    void on_actionNewPlaylist_triggered();
    void on_actionPlqyPlaylist_triggered();
    void on_actionImportPlaylist_triggered();
    void on_actionExportPlaylist_triggered();
    void on_actionDeletePlaylist_triggered();
    void on_actionPreferences_triggered();
    void on_actionInfo_triggered();
    void on_actionAdd_to_Library_triggered();  // cmd + O
    void on_actionDelete_from_Library_triggered();  // delete song from library
    void on_actionAddtoPlaylist_triggered();
    void playNext();
    void on_actionFast_forward_triggered();
    void on_actionRewind_triggered();
    void on_search_line_editingFinished();
    void on_songs_clicked();
    void on_modeButton_clicked();
    void on_upNextButton_clicked();

signals:
    void editTagsCompleted(const QModelIndex &, const Music &);
    void deleteSong(const QModelIndex &);

private:
    void readSettings();
    void writeSettings();
    void loadCoverImage(const QModelIndex &index);
    void setupMusicTableModel();
    void setupPlayListTableModel();
    Music getMusicfromTable();

    Ui::MainWindow *ui;

//    MediaLibrary *m_library = nullptr;
//    QItemSelectionModel *m_selection_model;
//    QString m_path;

    MusicTableModel *m_tableModel;
    Searcher *m_searcher;
    SoundPlayer *m_player;
    Settings *m_settings;
    QModelIndex m_table_index {};
    QModelIndex m_playList_index {};

    Music new_song_info;
    SqlBase *m_base;
    QSqlTableModel *m_SQL_model;
    QSqlTableModel *m_PlayList_model;
    StarDelegate *m_star_delegate;
    NextUp *nextUp {nullptr};

//    void add_to_library(const QString& file_name);
    void setMusicPlay(QString soundPath);
    void setMusicPlay(int index);
//    void outputCurrentInfo(const QVector<QString> &current, const QModelIndex &index);
};

#endif // MAINWINDOW_H
