#ifndef DIALOGINFO_H
#define DIALOGINFO_H

#include <QDialog>
#include "music.h"

namespace Ui {
class DialogInfo;
}

class DialogInfo : public QDialog
{
    Q_OBJECT

public:
    explicit DialogInfo(Music songInfo, QWidget *parent = nullptr);
    ~DialogInfo();

    void accepted();
    void coverInfoDoubleclicked();
    Music get_tag_changes(Music &music_tags);

public slots:
    void on_nextInfo_clicked();
    void on_prevInfo_clicked();

private:
    void load_cover();
    void setLines();

    Ui::DialogInfo *ui;
    Music m_tagsInfo;
};

#endif // DIALOGINFO_H
