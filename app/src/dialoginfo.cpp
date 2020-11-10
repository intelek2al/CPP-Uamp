#include <QFileDialog>
#include <QBuffer>
#include "dialoginfo.h"
#include "ui_dialoginfo.h"
#include "tag_functions.h"
#include "loggingcategories.h"
#include "string"

DialogInfo::DialogInfo(Music songInfo, QWidget *parent):
    QDialog(parent),
    ui(new Ui::DialogInfo)
{
    ui->setupUi(this);

    m_tagsInfo = songInfo;

    qInfo(logInfo()) << "DialogInfo()";
    ui->line_title->setText(songInfo.m_title);
    ui->line_artist->setText(songInfo.m_artist);
    ui->line_album->setText(songInfo.m_album);
    ui->line_genre->setText(songInfo.m_genre);
    ui->line_year->setText(songInfo.m_year);
    ui->line_track->setText(songInfo.m_track);
    ui->line_raiting->setText(songInfo.m_rate);
    ui->line_comments->setText(songInfo.m_comment);
    ui->line_path->setText(songInfo.m_path);
    ui->title_large->setText(songInfo.m_title);
    ui->artisti_large->setText(songInfo.m_artist);
    connect(ui->coverInfo, &ClickedLabel::doubleClicked, this, &DialogInfo::coverInfoDoubleclicked);
    load_cover();
//    qDebug(logDebug()) << "DialogInfo constuctor 4";
}


void DialogInfo::load_cover() {
    QImage coverQImg(":/def_cover_color.png");
    QPixmap outPixmap = QPixmap();

    if (m_tagsInfo.m_cover == nullptr) {
        outPixmap = (QPixmap::fromImage(coverQImg));
    }
    else {
        outPixmap.loadFromData(m_tagsInfo.m_cover);
    }
    ui->coverInfo->setPixmap(outPixmap);
}

DialogInfo::~DialogInfo()
{
    delete ui;
}


void DialogInfo::coverInfoDoubleclicked() {
    qDebug(logDebug()) << "coverInfoDoubleclicked";

    if (m_tagsInfo.m_path.isEmpty() || m_tagsInfo.m_name.isEmpty()) {
        return;
    }

    QString fileName = QFileInfo(m_tagsInfo.m_path).fileName();
    QString fileType = fileName.mid(fileName.lastIndexOf(".") + 1, -1);

    if (fileType == "mp3") {
        QString file_image = QFileDialog::getOpenFileName(
                this,
                tr("Open File"),
                "~/",
                tr("Images (*.png *.jpg)")
        );
        qDebug(logDebug()) << "new file name cover" << file_image;

        if (!(TagFunctions::set_image_mpeg(m_tagsInfo.m_path, file_image))) {
            qInfo(logInfo()) << m_tagsInfo.m_name <<  " not editable";
//            ui->statusbar->showMessage(m_tagsInfo[0] + "not editable", 2000);
        }

        QImage coverQImg(file_image);
        QByteArray byte_cover;
        QBuffer buffer(&byte_cover);
        buffer.open(QIODevice::WriteOnly);
        coverQImg.save(&buffer,"PNG");
        m_tagsInfo.m_cover = byte_cover;
        load_cover();  // reopen cover
        qInfo(logInfo()) << "cover changed";
    }
    else {
        qInfo(logInfo()) << "cover not editable";
    }
}

void DialogInfo::accepted() {
//    qDebug(logDebug()) << "ui->themeComboBox->currentText()" << ui->themeComboBox->currentText();
//    m_dialog_settings["theme"] = ui->themeComboBox->currentText();
//
//    emit SavedSettings(m_dialog_settings);
    QDialog::accept();
}

Music DialogInfo::get_tag_changes(Music &music_tags) {

    music_tags.m_name = m_tagsInfo.m_name;
    music_tags.m_time = m_tagsInfo.m_time;
    music_tags.m_rate = ui->line_raiting->text();
    music_tags.m_title = ui->line_title->text();
    music_tags.m_artist = ui->line_artist->text();
    music_tags.m_album = ui->line_album->text();
    music_tags.m_genre = ui->line_genre->text();
//    music_tags.m_year = std::stoi(ui->line_year->text().toStdString());
//    music_tags.m_track = std::stoi(ui->line_track->text().toStdString());
    music_tags.m_year = ui->line_year->text();
    music_tags.m_track = ui->line_track->text();
    music_tags.m_rate = ui->line_raiting->text();
    music_tags.m_comment = ui->line_comments->text();
    music_tags.m_cover = m_tagsInfo.m_cover;
    music_tags.m_path = m_tagsInfo.m_path;
    music_tags.m_url = m_tagsInfo.m_url;

    qDebug(logDebug()) << "music_tags.m_name " << music_tags.m_name;
    qDebug(logDebug()) << "music_tags.m_time " << music_tags.m_time;
    qInfo(logInfo()) << "music_tags " << music_tags.getStr();
    return Music();
}

//void DialogInfo::on_nextInfo_clicked()
//{
//
//}



