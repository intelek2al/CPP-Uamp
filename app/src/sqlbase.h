#ifndef UAMP_SQLBASE_H
#define UAMP_SQLBASE_H

#include <QString>
#include <QSqlDatabase>
#include "music.h"

class SqlBase {

public:
   SqlBase();

   ~SqlBase();

   bool AddtoLibrary(const QString& media_path);
   Music LoadSongTags(const QString &file_name);

private:
    bool createConnection();
    bool createNewBase();
    bool loadData();

    QSqlDatabase m_media_base;

};


#endif //UAMP_SQLBASE_H
