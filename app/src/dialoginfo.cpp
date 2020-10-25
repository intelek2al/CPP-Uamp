#include "dialoginfo.h"
#include "ui_dialoginfo.h"

DialogInfo::DialogInfo(QVector<QString> songInfo, QWidget *parent):
    QDialog(parent),
    ui(new Ui::DialogInfo)
{
    ui->setupUi(this);
    m_tagsInfo = songInfo;
}

DialogInfo::~DialogInfo()
{
    delete ui;
}
