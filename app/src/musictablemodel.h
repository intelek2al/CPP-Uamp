#ifndef MUSICTABLEMODEL_H
#define MUSICTABLEMODEL_H

#include <QMainWindow>
#include <QAbstractItemModel>
#include <QVector>
#include "music.h"


class MusicTableModel : public QAbstractTableModel
{
    Q_OBJECT
 public:
    explicit MusicTableModel(QWidget *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    void sort(int column, Qt::SortOrder order = Qt::AscendingOrder) override;
    void music_list_add(QVector<QVector<QString>> params);
    void music_list_add(const QVector<Music> &params);

 private:
    //    {"Name", "Time", "Title", "Artist", "Genre", "Album", "Year", "Track", "Path", "Comment" };

    QWidget *m_parent;
    QVector<QString> listHeaders = {"Name", "Time", "Title", "Artist", "Rating", "Genre", "Album", "Year", "Track", "Comment", "Path"};
    QHash<int, QByteArray> m_roleNames;
    QVector<Music> music_list;
//    /* QVector<QVector<QString>> music_list/*{{"bbb", "BBB", "R&B", "bbb", "BBB", "R&B"},
// *                                          {"ccasd", "CCC", "Rock", "ccc", "CCC", "Rock"},
// *                                          {"aaa", "AAA", "Rock", "aaa", "AAA", "Rock"},
// *                                          {"Killer", "Queen", "Jass", "aaa", "AAA", "Rock"}};*/
};

#endif // MUSICTABLEMODEL_H
