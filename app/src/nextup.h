#ifndef NEXTUP_H
#define NEXTUP_H

#include <QWidget>

namespace Ui {
class NextUp;
}

class NextUp : public QWidget
{
    Q_OBJECT

public:
    explicit NextUp(QWidget *parent = nullptr);
    ~NextUp();

private:
    Ui::NextUp *ui;
};

#endif // NEXTUP_H
