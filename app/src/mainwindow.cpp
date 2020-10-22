#include "mainwindow.h"
#include "sound_tags.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <algorithm>

//    {"Name", "Time", "Title", "Artist", "Genre", "Album", "Year", "Track", "Path", "Comment" };

//#define T___NAME 0
//#define T___TIME 1
//#define V__TITLE 2
//#define V_ARTIST 3
//#define V__GENRE 4
//#define V__ALBUM 5
//#define T___YEAR 6
//#define T__TRACK 7
//#define T___COMT 8

//#define V___NAME 0
//#define V___TIME 1
//#define V__TITLE 2
//#define V_ARTIST 3
//#define V__GENRE 4
//#define V__ALBUM 5
//#define V___YEAR 6
//#define V__TRACK 7
//#define V___COMT 8

#define default_cover ":/logo1.png"

using std::cout;
using std::endl;

char *toChar(QString str)
{
    char *test = str.toUtf8().data();
    return test;
}

void MainWindow::outputCurrentInfo(const QVector<QString> &current, const QModelIndex &index)
{
    m_tableViewer->setNewItems(current, index);
}

MainWindow::MainWindow(QString sPath, QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//    QResource::registerResource("./app/myresource.rcc");

    m_player = new SoundPlayer(ui);
    ui->playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    ui->pauseButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
    ui->stopButton->setIcon(style()->standardIcon(QStyle::SP_MediaStop));
    QImage def_cover(":/default_cover.png");
    QPixmap pix(QPixmap::fromImage(def_cover));
    ui->cover_label_large->setPixmap(pix);
    ui->cover_label->setPixmap(pix);


    m_tableViewer = new TableViewer(ui->tableInfoSong);
    m_dirmodel = new QFileSystemModel(this);
    m_dirmodel->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);
    m_dirmodel->setRootPath("~/");
    ui->log->setHidden(true);
    m_log = new Logger(ui);
    m_searcher = new Searcher{ui->search_line, ui->filterBox, &m_music_list};


    if (QDir c_dir(sPath); !c_dir.exists()) {
        m_path = "~/";
        m_log->add_log_massage(sPath + " directory not exists");
        ui->statusbar->showMessage(sPath + " directory not exists", 3000);
    }
    m_path = sPath;
    ui->fileBrowser->setModel(m_dirmodel);
    ui->fileBrowser->scrollTo(m_dirmodel->index(m_path));

    ui->verticalLayout_2->setAlignment(ui->cover_label_large, Qt::AlignmentFlag::AlignCenter);
    for (int i = 1; i < m_dirmodel->columnCount(); ++i)
    {
        ui->fileBrowser->hideColumn(i);
    }
    on_fileBrowser_clicked(m_dirmodel->index(m_path));

    connect(ui->filterBox, SIGNAL(activated(int)), this, SLOT(on_search_line_editingFinished()));
}

MainWindow::~MainWindow()
{
    delete m_tableViewer;
    delete m_dirmodel;
    delete m_searcher;
//    system("leaks -q utag");
    delete ui;
}

void MainWindow::readDir(const QModelIndex &index) {
    QString sPath = m_dirmodel->fileInfo(index).absoluteFilePath();
    QDir current_directory(sPath);

    if (!current_directory.isReadable())
    {
        m_log->add_log_massage(sPath + " directory not readable");
        ui->statusbar->showMessage(sPath + " directory not readable", 3000);
    };

    current_directory.setFilter(QDir::NoDotAndDotDot | QDir::Files);
    current_directory.setNameFilters(QStringList() << "*.mp3"
                                                   << "*.flac"
                                                   << "*.wav"
                                                   << "*.ogg"
                                                   << "*.m4a"
                                                   << "*.aif");
    //    current_directory.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    //    current_directory.setSorting(QDir::Size | QDir::Reversed);

    QFileInfoList list = current_directory.entryInfoList();

    m_music_list.clear();
    for (int i = 0; i < list.size(); ++i)
    {
        QFileInfo fileInfo = list.at(i);
        if (!fileInfo.isReadable()) {
            m_log->add_log_massage(fileInfo.fileName() + " not readable");
            ui->statusbar->showMessage(fileInfo.fileName()  + " not readable", 3000);
            continue;
        }
        QVector<QString> tmp;
        try
        {
            Sound_tags current;
            tmp = read_tags(toChar(QString(fileInfo.fileName())),
                            toChar(QString(fileInfo.filePath())));
        }
        catch (std::exception &e)
        {
            std::cerr << e.what() << std::endl;
        }
        if (!tmp.empty())
            m_music_list.push_back(tmp);
    }
}

void MainWindow::on_fileBrowser_clicked(const QModelIndex &index)
{
    readDir(index);

    if (!m_tableModel)
        delete m_tableModel;
    m_tableModel = new MusicTableModel(ui->mainMusicTable);
    m_searcher->setDown();
    m_tableModel->music_list_add(m_music_list);
    ui->mainMusicTable->setModel(m_tableModel);
}

void MainWindow::on_mainMusicTable_clicked(const QModelIndex &index)
{
    QVector<QString> current = m_music_list[index.row()];
    std::string fileName = m_music_list[index.row()][0].toStdString();
    std::string fileType = fileName.substr(fileName.size() - 3);
    QImage coverQImg;

    if (fileType == "mp3") {
        coverQImg = load_cover_image_mpeg(m_music_list[index.row()][8].toStdString().data());
        ui->statusbar->showMessage(tr( " loaded"), 200);
    }
    else if (fileType == "m4a") {
        coverQImg = load_cover_image_m4a(m_music_list[index.row()][8].toStdString().data());
        ui->statusbar->showMessage(tr( " loaded"), 200);
    }
    else {
        ui->statusbar->showMessage(tr(" cover is unsupported"), 200);
        m_log->add_log_massage(m_music_list[index.row()][0] + " cover is unsupported");
//        coverQImg = QImage("../../app/logo1.png");
        coverQImg = QImage(default_cover);
    }
    QPixmap pix(QPixmap::fromImage(coverQImg));
    ui->cover_label_large->setPixmap(pix);
    outputCurrentInfo(current, index);
}

void MainWindow::on_pushButton_clicked()
{
    auto newSongTag = m_tableViewer->getResult();

    if (newSongTag[8].isEmpty() || newSongTag[0].isEmpty()) {
        return;
    }
    if (!(modify_tags(newSongTag))) {
        ui->statusbar->showMessage(newSongTag[8] + " is not writable", 200);
        m_log->add_log_massage(newSongTag[8] + " is not writable");
    }

    m_music_list[m_tableViewer->getIndex().row()] = std::move(newSongTag);
    m_tableModel->music_list_add(m_music_list);
    ui->mainMusicTable->setModel(m_tableModel);
    ui->mainMusicTable->viewport()->update();
}

void MainWindow::on_mainMusicTable_doubleClicked(const QModelIndex &index)
{
    QVector<QString> current = m_music_list[index.row()];

    //    QImage coverQImg = load_cover_image(m_music_list[index.row()][8].toStdString().data());
    //    ui->statusbar->showMessage(tr("image loaded"), 200);
    //    QGraphicsScene *scene = new QGraphicsScene();
    //    ui->imageSong->setScene(scene);
    //    QGraphicsPixmapItem *item = new QGraphicsPixmapItem(QPixmap::fromImage(coverQImg));
    //    scene->addItem(item);
    //    ui->imageSong->show();
    outputCurrentInfo(current, index);
    setMusicPlay(current[8]);

    std::string fileName = m_music_list[index.row()][0].toStdString().data();
    std::string fileType = fileName.substr(fileName.size() - 3);
    QImage coverQImg;

    if (fileType == "mp3") {
        coverQImg = load_cover_image_mpeg(m_music_list[index.row()][8].toStdString().data());
        ui->statusbar->showMessage(tr( " loaded"), 200);
    }
    else if (fileType == "m4a") {
        coverQImg = load_cover_image_m4a(m_music_list[index.row()][8].toStdString().data());
        ui->statusbar->showMessage(tr( " loaded"), 200);
    }
    else {
        ui->statusbar->showMessage(tr(" cover is unsupported." ), 200);
//        coverQImg = QImage("../../app/logo1.png");  // Clion
        coverQImg = QImage("./app/logo1.png");  // zsh
    }

    QPixmap pix(QPixmap::fromImage(coverQImg));
    ui->cover_label->setPixmap(pix);
}

void MainWindow::setMusicPlay(QString soundPath)
{
    m_player->setSound(soundPath);
}

void MainWindow::on_playButton_clicked()
{
    m_player->setPlay();
}

void MainWindow::on_pauseButton_clicked()
{
    m_player->setPause();
}

void MainWindow::on_stopButton_clicked()
{
    m_player->setStop();
}

void MainWindow::on_statusPlay_valueChanged(int value)
{
    m_player->setMovedPosition(value);
}

void MainWindow::on_statusVolume_valueChanged(int value)
{
    m_player->setVolume(value);
}

void MainWindow::on_change_cover_button_clicked()
{
    auto currentSongTag = m_tableViewer->getResult();
    if (currentSongTag[8].isEmpty() || currentSongTag[0].isEmpty()) {
        return;
    }

    std::string current_file = currentSongTag[0].toStdString();
    std::string fileType = current_file.substr(current_file.size() - 3);

    if (fileType == "mp3") {
        QString file_image = QFileDialog::getOpenFileName(
                this,
                tr("Open File"),
                "~/",
                tr("Images (*.png *.jpg)")
        );
        if (!(set_image_mpeg(currentSongTag[8].toStdString().data(), file_image.toStdString().data()))) {
            m_log->add_log_massage(currentSongTag[8] + " not editable");
            ui->statusbar->showMessage(currentSongTag[0] + "not editable", 2000);
        }
        ui->statusbar->showMessage(tr(file_image.toStdString().data()), 2000);
    }
    /*
    else if (fileType == "m4a") {
        QString file_image = QFileDialog::getOpenFileName(
                this,
                tr("Open File"),
                "~/",
                tr("Images (*.png *.jpg)")
        );
        if (!(load_cover_image_m4a(currentSongTag[8].toStdString().data(), file_image.toStdString().data()))) {
            m_log->add_log_massage(currentSongTag[8] + " not editable");
            ui->statusbar->showMessage(currentSongTag[0] + "not editable", 2000);
        }
        ui->statusbar->showMessage(tr(file_image.toStdString().data()), 2000);
    }
     */
    else {
        ui->statusbar->showMessage(tr("cover not editable"), 2000);
    }
}

void MainWindow::on_actionlog_triggered()
{
    m_log->show_logger();
    ui->statusbar->showMessage(tr("menu log"), 2000);
}


void MainWindow::on_search_line_editingFinished()
{
    readDir(ui->fileBrowser->currentIndex());
    auto tmp = m_searcher->search();
    m_music_list = tmp;
    if (!m_tableModel)
        delete m_tableModel;
    m_tableModel = new MusicTableModel(ui->mainMusicTable);
    m_tableModel->music_list_add(m_music_list);
    ui->mainMusicTable->setModel(m_tableModel);
    ui->mainMusicTable->viewport()->update();
}
