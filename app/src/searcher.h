#ifndef SEARCHER_H
#define SEARCHER_H

#include <QObject>
#include <QLineEdit>
#include <QComboBox>
#include <QVector>
#include <QString>
#include <QMap>

class Searcher : public QObject
{
    Q_OBJECT
public:
    explicit Searcher(QLineEdit *parent = nullptr, QComboBox *hendler = nullptr, QVector<QVector<QString>> *tableMusic = nullptr);
    ~Searcher() override = default;
    void setDown();
    QVector<QVector<QString>> getList() const;

//public slots:
    QVector<QVector<QString>> search();

private:
    QString m_searchParam;
    QLineEdit *m_parent = nullptr;
    QComboBox *m_handler = nullptr;
    QVector<QVector<QString>> *m_list = nullptr;
    QVector<QVector<QString>> m_searched_list;
//  All  Name Title Artist Album Genre Year
//        0     2     3      5     4    6
    QMap<int, int> m_key{{0, -1},
                         {1, 0},
                         {2, 2},
                         {3, 3},
                         {4, 5},
                         {5, 4},
                         {6, 6}};
};

#endif // SEARCHER_H
