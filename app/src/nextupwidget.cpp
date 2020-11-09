#include "nextupwidget.h"
#include "ui_nextupwidget.h"
#include "tag_functions.h"
#include "loggingcategories.h"

NextUpWidget::NextUpWidget(QWidget *parent, Music song) :
    QWidget(parent),
    ui(new Ui::NextUpWidget),
    m_song(song)
{

    ui->setupUi(this);


    QImage coverQImg(":/def_cover_color.png");
//    auto cover = TagFunctions::load_cover_image(m_song.m_url.path().toStdString().data());
    QString fileType = QFileInfo(m_song.m_url.path().toStdString().data()).completeSuffix();

    if (fileType == "mp3") {
        coverQImg = TagFunctions::load_cover_image_mpeg(m_song.m_path.toStdString().data());
    }
    if (fileType == "m4a") {
        coverQImg = TagFunctions::load_cover_image_m4a(m_song.m_path.toStdString().data());
    }

    QPixmap pixmap(QPixmap::fromImage(coverQImg));
    QByteArray cover_array ;
//    coverQImg.loadFromData()

//    pixmap.convertFromImage(cover);
    ui->labelCover->setPixmap(pixmap);
    QFontMetrics metrics(ui->labelCover->font());
    QString artist = metrics.elidedText(m_song.m_artist, Qt::ElideRight, ui->labelArtist->width());
    QString title = metrics.elidedText(m_song.m_title, Qt::ElideRight, ui->labelSong->width());
    ui->labelArtist->setText(artist);
    ui->labelSong->setText(title);
}

NextUpWidget::~NextUpWidget()
{
    delete ui;
}

