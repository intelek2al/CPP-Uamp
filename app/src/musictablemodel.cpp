#include "musictablemodel.h"
#include "mainwindow.h"
#include "iostream"

MusicTableModel::MusicTableModel(QWidget *parent) : m_parent(parent)
{
    m_roleNames.insert(0, QByteArray("Title"));
    m_roleNames.insert(1, QByteArray("Artist"));
    m_roleNames.insert(2, QByteArray("Gener"));
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

int MusicTableModel::rowCount(const QModelIndex &) const
{
    return music_list.size();
}

int MusicTableModel::columnCount(const QModelIndex &) const
{
    return listHeaders.size();
}

QVariant MusicTableModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (!music_list.empty())
        {
//            std::cerr << "---\nrow: " << index.row() << "\ncolumn: " << index.column() << "\nfile: " << music_list[index.row()][0].toStdString() << "\n---\n\n";
            if (!music_list[index.row()][index.column()].isEmpty())
                return music_list[index.row()][index.column()];
        }
    }
    // else if (role == Qt::DecorationRole)
    // {
    //     if (!music_list.empty())
    //     {
    //         if (!music_list[index.row()][index.column()].isEmpty())
    //             return music_list[index.row()][index.column()];
    //     }
    // }
    // else if (role == Qt::StatusTipRole)
    // {
    //     if (!music_list.empty())
    //     {
    //         if (!music_list[index.row()][index.column()].isEmpty())
    //             return music_list[index.row()][index.column()];
    //     }
    // }
    return QString();
}

QVariant MusicTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();
//    static int row = 0;

    if (orientation == Qt::Horizontal)
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
    music_list = std::move(params);
}
