#ifndef DIALOGINFO_H
#define DIALOGINFO_H

#include <QDialog>

namespace Ui {
class DialogInfo;
}

class DialogInfo : public QDialog
{
    Q_OBJECT

public:
    explicit DialogInfo(QVector<QString> songInfo, QWidget *parent = nullptr);
    ~DialogInfo();

    private slots:
    void on_nextInfo_clicked();



private:
    Ui::DialogInfo *ui;
    QVector<QString> m_tagsInfo;
};

#endif // DIALOGINFO_H
