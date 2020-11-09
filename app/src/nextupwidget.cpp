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
    QPixmap outPixmap = QPixmap();

    if (song.m_cover == nullptr) {
        outPixmap = (QPixmap::fromImage(coverQImg));
    }
    else {
        outPixmap.loadFromData(song.m_cover);
    }

    ui->labelCover->setPixmap(outPixmap);
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
