#include "nextup.h"
#include "ui_nextup.h"
#include "nextupwidget.h"
#include "soundPlayer.h"

NextUp::NextUp(QWidget *parent, SoundPlayer *player) :
    QWidget(parent),
    ui(new Ui::NextUp),
    m_player(player) {
    ui->setupUi(this);

    setSongs();
}
#include <iostream>
void NextUp::setSongs() {
    if (!m_player)
        return;
    Playlist upNext = m_player->upNext();
    Playlist history = m_player->history();
    for (size_t i = 0; i < upNext.size(); ++i) {
        auto *__item = new QListWidgetItem;
        auto *__widget = new NextUpWidget(ui->listUpNext, upNext[i]);
        __widget->setMaximumSize(ui->listUpNext->width(), __widget->height());
        __item->setSizeHint(__widget->sizeHint());
        m_upNext[__item] = __widget;
        ui->listUpNext->addItem(__item);
        ui->listUpNext->setItemWidget(__item, __widget);
    }
    for (size_t i = 0; i < history.size(); ++i) {
        auto *__item = new QListWidgetItem;
        auto *__widget = new NextUpWidget(ui->listHistory, history[i]);
        __item->setSizeHint(__widget->sizeHint());
        m_history[__item] = __widget;
        ui->listHistory->addItem(__item);
        ui->listHistory->setItemWidget(__item, __widget);
    }
}

NextUp::~NextUp()
{

    delete ui;
}
