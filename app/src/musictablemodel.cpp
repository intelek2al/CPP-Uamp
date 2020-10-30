#include "musictablemodel.h"
#include "mainwindow.h"
#include "iostream"
#include "loggingcategories.h"
//#include "music.h"

MusicTableModel::MusicTableModel(QVector<Music>& _m_media_library, QWidget *parent)
        : m_parent(parent), m_media_library(_m_media_library)
{
}

int MusicTableModel::rowCount(const QModelIndex &) const
{
    return m_media_library.size();
}

int MusicTableModel::columnCount(const QModelIndex &) const {
    return listHeaders.size();

}

QVariant MusicTableModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (!m_media_library.empty())
        {
            if (!m_media_library[index.row()][index.column()].isEmpty())
                return m_media_library[index.row()][index.column()];
        }
    }
//    return QVariant();
    return QString();
}

bool MusicTableModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (index.isValid() && role == Qt::EditRole) {
        m_media_library[index.row()][index.column()] = value.toString();
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
//      {"Title", "Time", "Artist", "Rating", "Genre", "Album", "Year", "Track", "Comment", "Name","Path"};
    for (int i = 0; i < 9; ++i) {
        QModelIndex temp = index.sibling(index.row(), i);
        setData(temp, new_tags[i], Qt::EditRole);
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
        std::sort(m_media_library.begin(), m_media_library.end(), [=](auto a, auto b) { return a[column] < b[column]; });
    }
    else
        std::sort(m_media_library.begin(), m_media_library.end(), [=](auto a, auto b) { return a[column] > b[column]; });
    endResetModel();
    // if (m_parent)
    //     m_parent->update();
}

void MusicTableModel::music_list_add(QVector<QVector<QString>> params)
{
    qDebug(logDebug()) <<  "music_list_add";
    auto iter = params.begin();

    for (auto &el : m_media_library) {
        if (iter == params.end())
            break;
        el = std::move(*(iter++));
    }
}

void MusicTableModel::music_list_add(const QVector<Music> &params) {
    qDebug(logDebug()) <<  "music_list_add Qvector<music>";
    layoutChanged();
//    m_media_library = std::move(params);

}

Qt::ItemFlags MusicTableModel::flags(const QModelIndex &index) const {
    if (index.isValid())
//        return Qt::ItemIsEditable | QAbstractTableModel::flags(index);
        return  QAbstractTableModel::flags(index);  // not editible cell
}

bool MusicTableModel::insertRows(int row, int count, const QModelIndex &parent) {
    if (count < 1 || row < 0)
        return false;

    beginInsertRows(parent, row, row + count - 1);
    m_media_library[row] = Music();
    endInsertRows();
    layoutChanged();
    return true;
}

bool MusicTableModel::removeRows(int row, int count, const QModelIndex &parent) {
    if (count < 1 || row < 0)
        return false;

    beginRemoveRows(parent, row, row + count - 1);
    m_media_library.remove(row);
    endRemoveRows();
    layoutChanged();
    return true;
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