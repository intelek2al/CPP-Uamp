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
    explicit MusicTableModel(QVector<Music>& _m_media_library, QWidget *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
//
//    bool insertColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;
//    bool removeColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    void sort(int column, Qt::SortOrder order = Qt::AscendingOrder) override;

    void music_list_add(QVector<QVector<QString>> params);
    void music_list_add(const QVector<Music> &params);
    Music rowData(const QModelIndex &index);


signals:
    void editCompleted(const QString &);

public slots:
    void saveTags(const QModelIndex &index, const Music& new_tags);

private:
    QWidget *m_parent;

    QVector<QString> listHeaders = {"Title", "Time", "Artist", "Rating", "Genre", "Album", "Year", "Track", "Comment",
                                    "Name","Path"};
//    QHash<int, QByteArray> m_roleNames;
    QVector<Music>& m_media_library;
};

#endif // MUSICTABLEMODEL_H



///  for sql

/*
void showTable()
{
    connOpen();
    QSqlQueryModel * myModel=new QSqlQueryModel(ui->tableView);
    QSqlQuery select;
    if (!select.exec("select * from tab")) {
        QMessageBox::critical(this, tr("Error"), select.lastError().text());
    }
    else {
        myModel->setQuery(select);
        QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel(myModel); // create proxy
        proxyModel->setSourceModel(myModel);
        ui->tableView->setSortingEnabled(true); // enable sortingEnabled
        ui->tableView->setModel(proxyModel);
    }
    connClose();
}

*/


