#include "searcher.h"
#include <algorithm>

Searcher::Searcher(QLineEdit *parent, QComboBox *handler, QVector<QVector<QString>> *tableMusic)
    : m_parent(parent),
      m_handler(handler),
      m_list(tableMusic){
}

void Searcher::setDown()
{
    m_searched_list.clear();
    m_parent->clear();
}

//QVector<QVector<QString>> Searcher::getList() const
//{
//    if (m_searched_list.empty())
//        return *m_list;
//    return m_searched_list;
//}

QVector<QVector<QString>> Searcher::search() {
    QVector<QVector<QString>> searched_list;

    if (m_parent->text().isEmpty())
        return *m_list;
    for(auto iter_song = m_list->begin(); iter_song != m_list->end(); ++iter_song) {
//        for (auto iter_tag = iter_song->begin();iter_tag != iter_song->end(); ++iter_tag) {

        QVector<QString>::iterator tmp;
        if (m_key[m_handler->currentIndex()] == -1)
            tmp = std::find(iter_song->begin(), iter_song->end(), m_parent->text());
        else
            tmp = (iter_song->begin())[m_key[m_handler->currentIndex()]] == m_parent->text() ?
                    (iter_song->begin()) + m_key[m_handler->currentIndex()] :
                    iter_song->end();
        if (tmp != iter_song->end()) {
            searched_list.push_back(*iter_song);
        }
    }
    return searched_list;
}
