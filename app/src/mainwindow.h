#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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

#include <iostream>
#include "tag_functions.h"
#include "tableviewer.h"
#include "musictablemodel.h"
#include "sound_tags.h"
#include "soundPlayer.h"
#include "logger.h"
#include "searcher.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE

class SoundPlayer;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QString path = "~/", QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void on_fileBrowser_clicked(const QModelIndex &index);

    void on_mainMusicTable_clicked(const QModelIndex &index);

    void on_pushButton_clicked();

    void on_mainMusicTable_doubleClicked(const QModelIndex &index);

    void on_playButton_clicked();

    void on_pauseButton_clicked();

    void on_stopButton_clicked();

    void on_statusPlay_valueChanged(int value);

    void on_statusVolume_valueChanged(int value);

    void on_change_cover_button_clicked();

    void on_actionlog_triggered();

//    void on_pushButton_2_clicked();
    void on_search_line_editingFinished();

private:
    Ui::MainWindow *ui;
    QFileSystemModel *m_dirmodel;
    QVector<QVector<QString>> m_music_list;
    TableViewer *m_tableViewer = nullptr;
    MusicTableModel *m_tableModel;
    SoundPlayer *m_player;
    QString m_path;
    Logger *m_log;
    Searcher *m_searcher;

    void readDir(const QModelIndex &index);
    void setMusicPlay(QString soundPath);
    void outputCurrentInfo(const QVector<QString> &current, const QModelIndex &index);
};

char *toChar(QString str);

#endif // MAINWINDOW_H
