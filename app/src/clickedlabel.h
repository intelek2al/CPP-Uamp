#ifndef UAMP_CLICKEDLABEL_H
#define UAMP_CLICKEDLABEL_H

#include <QWidget>
#include <QLabel>

class ClickedLabel : public QLabel {

    Q_OBJECT

public:
    explicit ClickedLabel(QWidget *parent = 0);
    ~ClickedLabel();

    void mouseDoubleClickEvent(QMouseEvent *event);

signals:
    void doubleClicked();
};


#endif //UAMP_CLICKEDLABEL_H
