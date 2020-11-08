#include "nextup.h"
#include "ui_nextup.h"
#include "nextupwidget.h"

NextUp::NextUp(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NextUp)
{
    ui->setupUi(this);
    NextUpWidget *w = new NextUpWidget;
//    ui->
}

NextUp::~NextUp()
{
    delete ui;
}
