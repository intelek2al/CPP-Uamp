#include "musictablemodel.h"
#include "mainwindow.h"
#include "iostream"
#include "loggingcategories.h"
//#include "music.h"

MusicTableModel::MusicTableModel(QWidget *parent)
        : m_parent(parent)
{
}

int MusicTableModel::rowCount(const QModelIndex &) const
{
    return music_list.size();
}

int MusicTableModel::columnCount(const QModelIndex &) const {
    return listHeaders.size();

}

QVariant MusicTableModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (!music_list.empty())
        {
            if (!music_list[index.row()][index.column()].isEmpty())
                return music_list[index.row()][index.column()];
        }
    }
//    return QVariant();
    return QString();
}

bool MusicTableModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (index.isValid() && role == Qt::EditRole) {
        music_list[index.row()][index.column()] = value.toString();
        emit dataChanged(index, index);

//        qDebug(logDebug()) << "new data value before" << value.toString();

//        qDebug(logDebug()) << "new data after   save" << music_list[index.row()][index.column()];

        return true;
    }
    return false;
}

void MusicTableModel::saveTags(const QModelIndex &index, const Music &new_tags) {
    qDebug(logDebug()) << "saveTags" << new_tags.m_name;
//        0       1      2           3         4        5       6       7         8         9       10
//     "Name", "Time", "Title", "Artist", "Rating", "Genre", "Album", "Year", "Track", "Comment", "Path"};

    for (int i = 2; i < 10; ++i) {
        QModelIndex temp = index.sibling(index.row(), i);
        setData(temp, new_tags[i], Qt::EditRole);
//        qDebug(logDebug()) << "|-------index row  = " << temp.row();
//        qDebug(logDebug()) << "|-------index col  = " << temp.column();
    }
}

QVariant MusicTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        try
        {
            listHeaders.at(section);
            return listHeaders[section];
        }
        catch (std::out_of_range &)
        {
        }
    }
    return QVariant();
}

void MusicTableModel::sort(int column, Qt::SortOrder order)
{
    beginResetModel();
    if (order == Qt::AscendingOrder)
    {
        std::sort(music_list.begin(), music_list.end(), [=](auto a, auto b) { return a[column] < b[column]; });
    }
    else
        std::sort(music_list.begin(), music_list.end(), [=](auto a, auto b) { return a[column] > b[column]; });
    endResetModel();
    // if (m_parent)
    //     m_parent->update();
}

void MusicTableModel::music_list_add(QVector<QVector<QString>> params)
{
    qDebug(logDebug()) <<  "music_list_add";
    auto iter = params.begin();

    for (auto &el : music_list) {
        if (iter == params.end())
            break;
        el = std::move(*(iter++));
    }
}

void MusicTableModel::music_list_add(const QVector<Music> &params) {
    qDebug(logDebug()) <<  "music_list_add Qvector<music>";
    music_list = std::move(params);
}

Qt::ItemFlags MusicTableModel::flags(const QModelIndex &index) const {
    if (index.isValid())
//        return Qt::ItemIsEditable | QAbstractTableModel::flags(index);
        return  QAbstractTableModel::flags(index);  // not editible cell
}

bool MusicTableModel::insertRows(int row, int count, const QModelIndex &parent) {
    beginInsertRows(parent, row, row + count - 1);
    endInsertRows();
    return true;
}

bool MusicTableModel::removeRows(int row, int count, const QModelIndex &parent) {
    return QAbstractItemModel::removeRows(row, count, parent);
}

bool MusicTableModel::insertColumns(int column, int count, const QModelIndex &parent) {
    return QAbstractItemModel::insertColumns(column, count, parent);
}

bool MusicTableModel::removeColumns(int column, int count, const QModelIndex &parent) {
    return QAbstractItemModel::removeColumns(column, count, parent);
}


// QModelIndex index(int row, int column, const QModelIndex &parent)
// {
//     return createIndex(row, column, /*&(music_list[index.row()].title)*/ "hello");
// }

// QModelIndex MusicTableModel::parent(const QModelIndex &child) const
// {
//     return QModelIndex();
// }

// QHash<int, QByteArray> MusicTableModel::roleNames() const
// {
//     return m_roleNames;
// }