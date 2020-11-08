#ifndef NEXTUP_H
#define NEXTUP_H

#include <QWidget>
#include <map>
#include <QListWidgetItem>

namespace Ui {
class NextUp;
}

class NextUpWidget;
class SoundPlayer;

class NextUp : public QWidget
{
    Q_OBJECT

public:
    explicit NextUp(QWidget *parent = nullptr, SoundPlayer *player = nullptr);
    ~NextUp();
    void setSongs();

private:
    Ui::NextUp *ui;
    SoundPlayer *m_player;
    std::map<QListWidgetItem *, NextUpWidget *> m_upNext;
    std::map<QListWidgetItem *, NextUpWidget *> m_history;
};

#endif // NEXTUP_H
