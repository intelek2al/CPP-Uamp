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

#include <iostream>
void Searcher::search() {
    m_searchParam = m_parent->text();

    if (m_searchParam.isEmpty()) {
        m_list->setFilter("");
        m_list->select();
        return;
    }
//    if ()
    QString filter = "";
    m_searchParam = " LIKE '%" + m_searchParam + "%'";
    if (m_handler->currentIndex() == 0) {
        for (int i = 1; i < m_box.size(); ++i) {
            filter += m_box[i] + m_searchParam;
            if (i != m_box.size() - 1)
                filter += " OR ";
        }
        std::cout << "FILTRE " << filter.toStdString() << std::endl;
    } else {
        filter = m_box[m_handler->currentIndex()];
        filter += m_searchParam;
    }
    m_list->setFilter(filter);
    m_list->select();
}
