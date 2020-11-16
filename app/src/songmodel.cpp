#include "songmodel.h"
#include <QSqlQueryModel>
#include "singletonbase.h"

Qt::ItemFlags SongModel::flags(const QModelIndex &index) const {
    Qt::ItemFlags flags = QSqlQueryModel::flags(index);
    if (index.column() == 1 || index.column() == 3 || index.column() == 4 || index.column() == 5
        || index.column() == 6 || index.column() == 7 || index.column() == 8 || index.column() == 9)
        flags |= Qt::ItemIsEditable;
    return flags;
}

SongModel::SongModel(QObject *parent, QSqlDatabase db) : QSqlTableModel(parent, db) {

}

