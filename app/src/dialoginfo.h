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

    void coverInfoDoubleclicked();

private:
    void load_cover();
    Ui::DialogInfo *ui;
    QVector<QString> m_tagsInfo;
};

#endif // DIALOGINFO_H
