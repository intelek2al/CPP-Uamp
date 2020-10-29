#include <QWidget>
#include <QStyledItemDelegate>

#ifndef UAMP_STARDELEGATE_H
#define UAMP_STARDELEGATE_H


class StarDelegate : public QStyledItemDelegate {
    Q_OBJECT

public:
    StarDelegate(QWidget *parent = 0);
    void paint(QPainter *painterm, const QStyleOptionViewItem &option,
               const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const;

};


#endif //UAMP_STARDELEGATE_H
