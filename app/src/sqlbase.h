#ifndef UAMP_SQLBASE_H
#define UAMP_SQLBASE_H

#include <QString>
#include <QSqlDatabase>

class SqlBase {

public:
   SqlBase();

   ~SqlBase();

private:

    bool createConnection();
    bool createNewBase();

//    QString m_library_name;
    QSqlDatabase m_media_base;

};


#endif //UAMP_SQLBASE_H
