#include "musictablemodel.h"
#include "mainwindow.h"
#include "iostream"
#include "loggingcategories.h"
#include "starrating.h"
#include "stardelegate.h"
//#include "music.h"

MusicTableModel::MusicTableModel(MediaLibrary& _m_library, QWidget *parent)
        : m_parent(parent), m_class_library(_m_library)
{
}

int MusicTableModel::rowCount(const QModelIndex &) const
{
    return m_class_library.m_media_list.size();
}

int MusicTableModel::columnCount(const QModelIndex &) const {
    return listHeaders.size();

}

QVariant MusicTableModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (!m_class_library.m_media_list.empty())
        {
            if (!m_class_library.m_media_list[index.row()][index.column()].isEmpty())
              if (index.column() == 3) {
//                  QTableWidgetItem *item3 = new QTableWidgetItem;
//                  item3->setData((0, QVariant::fromValue(StarRating(m_class_library.m_media_list[index.row()].m_rate.toInt())));
//                  m_parent->setItem(row, 3, item3);
//                  setData(0, QVariant::fromValue(StarRating(m_class_library.m_media_list[index.row()].m_rate.toInt())));
//                int rate =
                return m_class_library.m_media_list[index.row()][index.column()].toInt();
              }
                return m_class_library.m_media_list[index.row()][index.column()];
        }
    }
//    return QVariant();
    return QString();
}


//item3->setData(0,QVariant::fromValue(StarRating(staticData[row].rating)));


bool MusicTableModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (index.isValid() && role == Qt::EditRole) {
        m_class_library.m_media_list[index.row()][index.column()] = value.toString();
        if (index.column() == 3) {
            qDebug(logDebug()) << "new data value rate =" << value.toString();
        }
//        setData(0, QVariant::fromValue(StarRating(m_class_library.m_media_list[index.row()].m_rate.toInt())));
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

Qt::ItemFlags MusicTableModel::flags(const QModelIndex &index) const {
    if (index.isValid())
        return  QAbstractTableModel::flags(index);
    return Qt::ItemFlags::enum_type::ItemIsEditable;
}

bool MusicTableModel::insertRows(int row, int count, const QModelIndex &parent) {
    if (count < 1 || row < 0)
        return false;

    beginInsertRows(parent, row, row + count - 1);
  m_class_library.m_media_list[row] = Music();
    endInsertRows();
    layoutChanged();
    return true;
}

bool MusicTableModel::removeRows(int row, int count, const QModelIndex &parent) {
    if (count < 1 || row < 0)
        return false;

    beginRemoveRows(parent, row, row + count - 1);
    qDebug(logDebug()) << "m_class_library.m_media_list size =" << m_class_library.m_media_list.size();
    m_class_library.m_media_list.remove(row);

    qDebug(logDebug()) << "m_class_library.m_media_list size after delrow =" << m_class_library.m_media_list.size();
    endRemoveRows();
    layoutChanged();
    return true;
}

void MusicTableModel::sort(int column, Qt::SortOrder order)
{
    beginResetModel();
    if (order == Qt::AscendingOrder)
    {
        std::sort(m_class_library.m_media_list.begin(), m_class_library.m_media_list.end(),
                  [=] (auto a, auto b) { return a[column] < b[column]; });
    }
    else
        std::sort(m_class_library.m_media_list.begin(), m_class_library.m_media_list.end(),
                  [=](auto a, auto b) { return a[column] > b[column]; });
    endResetModel();
    emit layoutChanged();
    // if (m_parent)
    //     m_parent->update();
}

//



//void MusicTableModel::music_list_add(const QVector<Music> &params) {
//    qDebug(logDebug()) <<  "music_list_add Qvector<music>";
//    layoutChanged();
////    m_media_library = std::move(params);
//
//}



//void MusicTableModel::music_list_add(QVector<QVector<QString>> params)
//{
//    qDebug(logDebug()) <<  "music_list_add";
//    auto iter = params.begin();
//
//    for (auto &el : m_media_library) {
//        if (iter == params.end())
//            break;
//        el = std::move(*(iter++));
//    }
//}

