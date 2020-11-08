#include "nextupwidget.h"
#include "ui_nextupwidget.h"

NextUpWidget::NextUpWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NextUpWidget)
{
    ui->setupUi(this);
}

NextUpWidget::~NextUpWidget()
{
    delete ui;
}
