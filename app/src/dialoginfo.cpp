#include <QFileDialog>
#include "dialoginfo.h"
#include "ui_dialoginfo.h"
#include "tag_functions.h"
#include "loggingcategories.h"

//    {"Name", "Time", "Title", "Artist", "Genre", "Album", "Year", "Track", "Path", "Comment" };

DialogInfo::DialogInfo(Music songInfo, QWidget *parent):
    QDialog(parent),
    ui(new Ui::DialogInfo)
{
    ui->setupUi(this);
    m_tagsInfo = songInfo;

//    ui->line_title->setText(songInfo[2]);
//    ui->line_artist->setText(songInfo[3]);
//    ui->line_album->setText(songInfo[5]);
//    ui->line_genre->setText(songInfo[4]);
//    ui->line_year->setText(songInfo[6]);
//    ui->line_track->setText(songInfo[7]);
//    ui->line_comments->setText(songInfo[9]);
//    ui->line_path->setText(songInfo[8]);
//
//    ui->title_large->setText(songInfo[2]);
//    ui->artisti_large->setText(songInfo[3]);


    connect(ui->coverInfo, &ClickedLabel::doubleClicked, this, &DialogInfo::coverInfoDoubleclicked);

    load_cover();

}


void DialogInfo::load_cover() {
    // cover
    std::string fileName = m_tagsInfo[0].toStdString();
    std::string fileType = fileName.substr(fileName.size() - 3);

    QImage coverQImg;
    if (fileType == "mp3") {
        coverQImg = load_cover_image_mpeg(m_tagsInfo[8].toStdString().data());
//        ui->statusbar->showMessage(tr( " loaded"), 200);
    }
    else if (fileType == "m4a") {
        coverQImg = load_cover_image_m4a(m_tagsInfo[8].toStdString().data());
//        ui->statusbar->showMessage(tr( " loaded"), 200);
    }
    else {
//        ui->statusbar->showMessage(tr(" cover is unsupported"), 200);
        coverQImg = QImage(":/def_cover_color.png");
//        coverQImg = QImage(default_cover);
    }
    QPixmap pix(QPixmap::fromImage(coverQImg));
    ui->coverInfo->setPixmap(pix);


//    QPixmap pix(QPixmap::fromImage(coverQImg));
//    ui->cover_label->setPixmap(pix);

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



void DialogInfo::coverInfoDoubleclicked() {
    qDebug(logDebug()) << "coverInfoDoubleclicked";

    if (m_tagsInfo[8].isEmpty() || m_tagsInfo[0].isEmpty()) {
        return;
    }

    std::string current_file = m_tagsInfo[0].toStdString();
    std::string fileType = current_file.substr(current_file.size() - 3);

    if (fileType == "mp3") {
        QString file_image = QFileDialog::getOpenFileName(
                this,
                tr("Open File"),
                "~/",
                tr("Images (*.png *.jpg)")
        );
        qDebug(logDebug()) << "new file name cover" << file_image;

        if (!(set_image_mpeg(m_tagsInfo[8].toStdString().data(), file_image.toStdString().data()))) {
            qInfo(logInfo()) << m_tagsInfo[0] <<  " not editable";
//            ui->statusbar->showMessage(m_tagsInfo[0] + "not editable", 2000);
        }
        load_cover();  // reopen cover
    }

//    else if (fileType == "m4a") {
//        QString file_image = QFileDialog::getOpenFileName(
//                this,
//                tr("Open File"),
//                "~/",
//                tr("Images (*.png *.jpg)")
//        );
//        if (!(load_cover_image_m4a(currentSongTag[8].toStdString().data(), file_image.toStdString().data()))) {
//            m_log->add_log_massage(currentSongTag[8] + " not editable");
//            ui->statusbar->showMessage(currentSongTag[0] + "not editable", 2000);
//        }
//        ui->statusbar->showMessage(tr(file_image.toStdString().data()), 2000);
//    }
    else {
        qInfo(logInfo()) << "cover not editable";
    }
}




//void DialogInfo::on_nextInfo_clicked()
//{
//
//}



