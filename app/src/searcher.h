#ifndef SEARCHER_H
#define SEARCHER_H

#include <QObject>
#include <QLineEdit>
#include <QComboBox>
#include <QVector>
#include <QString>
#include <QMap>
#include <QSqlTableModel>
class Searcher : public QObject
{
    Q_OBJECT
public:
    explicit Searcher(QLineEdit *parent = nullptr, QComboBox *hendler = nullptr, QSqlTableModel *tableMusic = nullptr);
    ~Searcher() override = default;
    void setDown();

//public slots:
    void search();

private:
    QString m_searchParam;
    QLineEdit *m_parent = nullptr;
    QComboBox *m_handler = nullptr;
    QSqlTableModel *m_list;
    QVector<QString> m_box = {"* = ", "Name = ", "Title = ", "Artist = ", "Album = ", "Genre = ", "Year = ", "Rating = "};
};

#endif // SEARCHER_H
