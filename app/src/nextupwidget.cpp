#include "nextupwidget.h"
#include "ui_nextupwidget.h"
#include "tag_functions.h"

NextUpWidget::NextUpWidget(QWidget *parent, Music song) :
    QWidget(parent),
    ui(new Ui::NextUpWidget),
    m_song(song)
{
    ui->setupUi(this);
    auto cover = TagFunctions::load_cover_image(m_song.m_url.path().toStdString().data());
    QPixmap pixmap;
    pixmap.convertFromImage(cover);
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
