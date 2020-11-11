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
    setLines();
}

void DialogInfo::setLines() {
    qInfo(logInfo()) << "DialogInfo()";
    ui->line_title->setText(m_tagsInfo.m_title);
    ui->line_artist->setText(m_tagsInfo.m_artist);
    ui->line_album->setText(m_tagsInfo.m_album);
    ui->line_genre->setText(m_tagsInfo.m_genre);
    ui->line_year->setText(m_tagsInfo.m_year);
    ui->line_track->setText(m_tagsInfo.m_track);
    ui->line_raiting->setText(m_tagsInfo.m_rate);
    ui->line_comments->setText(m_tagsInfo.m_comment);
    ui->line_path->setText(m_tagsInfo.m_path);
    ui->title_large->setText(m_tagsInfo.m_title);
    ui->artisti_large->setText(m_tagsInfo.m_artist);
    ui->line_lyrics->setText(m_tagsInfo.m_lyrics);
    connect(ui->coverInfo, &ClickedLabel::doubleClicked, this, &DialogInfo::coverInfoDoubleclicked);
    load_cover();
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
    music_tags.m_year = ui->line_year->text();
    music_tags.m_track = ui->line_track->text();
    music_tags.m_rate = ui->line_raiting->text();
    music_tags.m_comment = ui->line_comments->text();
    music_tags.m_cover = m_tagsInfo.m_cover;
    music_tags.m_path = m_tagsInfo.m_path;
    music_tags.m_lyrics = ui->line_lyrics->toPlainText();
    music_tags.m_url = m_tagsInfo.m_url;
    return Music();
}


void DialogInfo::on_nextInfo_clicked()
{
    qDebug(logDebug()) << "DialogInfo::on_nextInfo_clicked";

}

void DialogInfo::on_prevInfo_clicked()
{
    qDebug(logDebug()) << "DialogInfo::on_prevInfo_clicked";
}

