#include "dialoginfo.h"
#include "ui_dialoginfo.h"
#include "tag_functions.h"

//    {"Name", "Time", "Title", "Artist", "Genre", "Album", "Year", "Track", "Path", "Comment" };

DialogInfo::DialogInfo(QVector<QString> songInfo, QWidget *parent):
    QDialog(parent),
    ui(new Ui::DialogInfo)
{
    ui->setupUi(this);
    m_tagsInfo = songInfo;

    ui->line_title->setText(songInfo[2]);
    ui->line_artist->setText(songInfo[3]);
    ui->line_album->setText(songInfo[5]);
    ui->line_genre->setText(songInfo[4]);
    ui->line_year->setText(songInfo[6]);
    ui->line_track->setText(songInfo[7]);
    ui->line_comments->setText(songInfo[9]);
    ui->line_path->setText(songInfo[8]);

    ui->title_large->setText(songInfo[2]);
    ui->artisti_large->setText(songInfo[3]);


    // cover
    std::string fileName = songInfo[0].toStdString();
    std::string fileType = fileName.substr(fileName.size() - 3);

    QImage coverQImg;
    if (fileType == "mp3") {
        coverQImg = load_cover_image_mpeg(songInfo[8].toStdString().data());
//        ui->statusbar->showMessage(tr( " loaded"), 200);
    }
    else if (fileType == "m4a") {
        coverQImg = load_cover_image_m4a(songInfo[8].toStdString().data());
//        ui->statusbar->showMessage(tr( " loaded"), 200);
    }
    else {
//        ui->statusbar->showMessage(tr(" cover is unsupported"), 200);
        coverQImg = QImage(":/def_cover_color.png");
//        coverQImg = QImage(default_cover);
    }
    QPixmap pix(QPixmap::fromImage(coverQImg));
    ui->coverInfo->setPixmap(pix);
//    outputCurrentInfo(current, index);

//    QImage def_cover(":/def_cover_color.png");
//    QPixmap pix(QPixmap::fromImage(def_cover));
//    ui->cover_label_large->setPixmap(pix);
//    ui->cover_label->setPixmap(pix);

}

DialogInfo::~DialogInfo()
{
    delete ui;
}

void DialogInfo::on_nextInfo_clicked()
{

}
