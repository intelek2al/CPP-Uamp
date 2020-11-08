#include "nextup.h"
#include "ui_nextup.h"
#include "nextupwidget.h"
#include <QTabBar>

NextUp::NextUp(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NextUp) {
    ui->setupUi(this);

    QListWidgetItem *item = new QListWidgetItem;
    NextUpWidget *widget = new NextUpWidget;

    item->setSizeHint(widget->sizeHint());

    ui->listUpNext->addItem(item);
    ui->listUpNext->setItemWidget(item, widget);
}

NextUp::~NextUp()
{
    delete ui;
}
