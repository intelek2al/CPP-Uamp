#include "musictablemodel.h"
#include "mainwindow.h"
#include "iostream"
#include "loggingcategories.h"
//#include "music.h"

//MusicTableModel::MusicTableModel(QWidget *parent) : m_parent(parent)
//{
//    m_roleNames.insert(0, QByteArray("Title"));
//    m_roleNames.insert(1, QByteArray("Artist"));
//    m_roleNames.insert(2, QByteArray("Gener"));
//}

MusicTableModel::MusicTableModel(QWidget *parent)
        : m_parent(parent)
{
}


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
            if (!music_list[index.row()][index.column()].isEmpty())
                return music_list[index.row()][index.column()];
        }
    }
    return QVariant();
//    return QString();
}

bool MusicTableModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (index.isValid() && role == Qt::EditRole) {

        qDebug(logDebug()) << "Qvariant " << value.toString();
        music_list[index.row()][index.column()] = value.toString();
//        std::cout << value.toString().toStdString() << " :\t";
//        std::cout << music_list[index.row()][index.column()] << " ";
//        QString result;
//        for (int row = 0; row < rowCount(); row++) {
//            for (int col= 0; col < columnCount(); col++)
//                result += music_list[row][col] + ' ';
//        }
//        emit editCompleted(result);
//        beginResetModel();
//        endResetModel();
        return true;
    }
    return false;
}

void MusicTableModel::saveTags(const QModelIndex &index, const Music &new_tags) {

    qDebug(logDebug()) << "saveTags" << new_tags.m_name;
//    qDebug(logDebug()) << "index row  = " << index.row();
//    qDebug(logDebug()) << "index col  = " << index.column();
    for (int i = 0; i < this->columnCount(); ++i) {
        QModelIndex temp = index.sibling(index.row(), i);
//        std::cout << setData(temp, new_tags[i], Qt::EditRole) << std::endl;
//        std::cout << new_tags[index.column()].toStdString() << " ";

        qDebug(logDebug()) << "|-------index row  = " << temp.row();
        qDebug(logDebug()) << "|-------index col  = " << temp.column();
    }

    emit dataChanged(QAbstractItemModel::createIndex(0, 0), QAbstractItemModel::createIndex(rowCount(), columnCount()));
//    qobject_cast<QTableView *>(m_parent)->viewport()->update();
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
    return Qt::ItemIsEditable | QAbstractTableModel::flags(index);
}

bool MusicTableModel::insertRows(int row, int count, const QModelIndex &parent) {

    return QAbstractItemModel::insertRows(row, count, parent);
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