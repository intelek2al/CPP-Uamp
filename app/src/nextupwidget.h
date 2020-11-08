#ifndef NEXTUPWIDGET_H
#define NEXTUPWIDGET_H

#include <QWidget>

namespace Ui {
class NextUpWidget;
}

class NextUpWidget : public QWidget
{
    Q_OBJECT

public:
    explicit NextUpWidget(QWidget *parent = nullptr);
    ~NextUpWidget();

private:
    Ui::NextUpWidget *ui;
};

#endif // NEXTUPWIDGET_H
