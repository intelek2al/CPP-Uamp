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
    explicit MusicTableModel(QWidget *parent = nullptr, QVector<Music> &_media = QVector<Music>());
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
//
    bool insertColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    void sort(int column, Qt::SortOrder order = Qt::AscendingOrder) override;

    void music_list_add(QVector<QVector<QString>> params);
    void music_list_add(const QVector<Music> &params);


signals:
    void editCompleted(const QString &);

public slots:
    void saveTags(const QModelIndex &index, const Music& new_tags);

private:
    QWidget *m_parent;

    QVector<QString> listHeaders = {"Name", "Time", "Title", "Artist", "Rating", "Genre", "Album", "Year", "Track", "Comment", "Path"};
    QHash<int, QByteArray> m_roleNames;
    QVector<Music>& music_list;

//    /* QVector<QVector<QString>> music_list/*{{"bbb", "BBB", "R&B", "bbb", "BBB", "R&B"},
// *                                          {"ccasd", "CCC", "Rock", "ccc", "CCC", "Rock"},
// *                                          {"aaa", "AAA", "Rock", "aaa", "AAA", "Rock"},
// *                                          {"Killer", "Queen", "Jass", "aaa", "AAA", "Rock"}};*/
};

#endif // MUSICTABLEMODEL_H
