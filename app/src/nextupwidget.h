#ifndef NEXTUPWIDGET_H
#define NEXTUPWIDGET_H

#include <QWidget>
#include "music.h"

namespace Ui {
class NextUpWidget;
}

class NextUpWidget : public QWidget
{
    Q_OBJECT

public:
    explicit NextUpWidget(QWidget *parent = nullptr, Music song = Music());
    ~NextUpWidget();

private:
    Ui::NextUpWidget *ui;
    Music m_song;
};

#endif // NEXTUPWIDGET_H
