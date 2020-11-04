#include "searcher.h"
#include <algorithm>

Searcher::Searcher(QLineEdit *parent, QComboBox *handler, QSqlTableModel *tableMusic)
    : m_parent(parent),
      m_handler(handler),
      m_list(tableMusic){
}

void Searcher::setDown()
{
    m_list->setFilter("");
    m_list->select();
    m_parent->clear();
}

//QVector<QVector<QString>> Searcher::getList() const
//{
//    if (m_searched_list.empty())
//        return *m_list;
//    return m_searched_list;
//}

void Searcher::search() {
    m_searchParam = m_parent->text();
    if (m_searchParam.isEmpty()) {
        m_list->setFilter("");
        m_list->select();
        return;
    }
    QString filter = m_box[m_handler->currentIndex()];
    filter += "'" + m_searchParam + "'";
    m_list->setFilter(filter);
    m_list->select();
}
