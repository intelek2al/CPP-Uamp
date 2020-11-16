#ifndef UAMP_SONGMODEL_H
#define UAMP_SONGMODEL_H

#include <QSqlTableModel>
#include <QSqlDatabase>

class SongModel : public QSqlTableModel {
    Q_OBJECT

public:
    SongModel(QObject* parent = 0, QSqlDatabase db = QSqlDatabase());

//    void clear();
//    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const ;
    Qt::ItemFlags flags(const QModelIndex &index) const;
//    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
//    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());
//    bool removeColumns(int column, int count, const QModelIndex &parent = QModelIndex());
//    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
//    int	rowCount(const QModelIndex &parent = QModelIndex()) const;
//    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
//    void sort(int column, Qt::SortOrder order);
};

#endif //UAMP_SONGMODEL_H
