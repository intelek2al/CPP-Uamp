#include "songquerymodel.h"
#include <QSqlQueryModel>
#include <QSqlQuery>
#include "singletonbase.h"


SongQueryModel::SongQueryModel(QObject *parent)
    : QSqlQueryModel(parent)
{
}


Qt::ItemFlags SongQueryModel::flags( const QModelIndex &index) const
{
  Qt::ItemFlags flags = QSqlQueryModel::flags(index);
  if (index.column() == 1 || index.column() == 3 || index.column() == 4 || index.column() == 5
      || index.column() == 6 || index.column() == 7 || index.column() == 8 || index.column() == 9)
    flags |= Qt::ItemIsEditable;
  return flags;
}



bool SongQueryModel::setData(const QModelIndex &index, const QVariant &value, int /* role */)
{
  if (index.column() < 1 || index.column() > 9)
    return false;

  QModelIndex primaryKeyIndex = QSqlQueryModel::index(index.row(), 0);
  int id = data(primaryKeyIndex).toInt();

  clear();

  bool ok;
  if (index.column() == 1) {
    ok = setFirstName(id, value.toString());
  } else {
    ok = setLastName(id, value.toString());
  }
  refresh();
  return ok;
}

bool SongQueryModel::setFirstName(int personId, const QString &firstName)
{
  QSqlQuery query;
  query.prepare("update person set firstname = ? where id = ?");
  query.addBindValue(firstName);
  query.addBindValue(personId);
  return query.exec();
}

bool SongQueryModel::setLastName(int personId, const QString &lastName)
{
  QSqlQuery query;
  query.prepare("update person set lastname = ? where id = ?");
  query.addBindValue(lastName);
  query.addBindValue(personId);
  return query.exec();
}

void SongQueryModel::refresh()
{
  setQuery("select * from person");
  setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
  setHeaderData(1, Qt::Horizontal, QObject::tr("First name"));
  setHeaderData(2, Qt::Horizontal, QObject::tr("Last name"));
}
