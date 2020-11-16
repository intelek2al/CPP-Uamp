#ifndef UAMP_APP_SRC_SONGQUERYMODEL_H_
#define UAMP_APP_SRC_SONGQUERYMODEL_H_

#include <QSqlQueryModel>


class SongQueryModel : public QSqlQueryModel {
 Q_OBJECT

 public:
  SongQueryModel(QObject *parent = 0);

  Qt::ItemFlags flags(const QModelIndex &index) const;
  bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

 private:
  bool setFirstName(int personId, const QString &firstName);
  bool setLastName(int personId, const QString &lastName);
  void refresh();
};

#endif //UAMP_APP_SRC_SONGQUERYMODEL_H_
