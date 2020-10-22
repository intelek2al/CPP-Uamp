#include "tableviewer.h"

TableViewer::TableViewer(QTableWidget *parent) : m_parent(parent)
{
    for (int i = 0; i < TAG_SIZE; ++i) {
        m_tagItemList[i] = new QTableWidgetItem("");
        parent->setItem(i, 0, m_tagItemList[i]);
    }
}

TableViewer::~TableViewer()
{
    for (int i = 0; i < TAG_SIZE; ++i) {
        delete m_tagItemList[i];
    }
}

void TableViewer::setNewItems(const QVector<QString> &current)
{
    for (int i = 0; i < TAG_SIZE; ++i) {
        m_tagItemList[i]->setData(Qt::DisplayRole, current[i]);
    }
}

void TableViewer::setNewItems(const QVector<QString> &current, const QModelIndex &index)
{
    m_index = index;
    for (int i = 0; i < TAG_SIZE; ++i) {
        m_tagItemList[i]->setData(Qt::DisplayRole, current[i]);
    }
}

QVector<QString> TableViewer::getResult()
{
    QVector<QString> newSongTag;
    for (int i = 0; i < TAG_SIZE; ++i) {
        newSongTag.push_back(QString(m_tagItemList[i]->data(Qt::DisplayRole).value<QString>()));
    }
    return newSongTag;
}

QModelIndex TableViewer::getIndex()
{
    return m_index;
}
