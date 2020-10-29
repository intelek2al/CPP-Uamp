#ifndef UAMP_MEDIALIBRARY_H
#define UAMP_MEDIALIBRARY_H

#include <QString>
#include <QVector>
#include "playlist.h"
//#include "music.h"

class MediaLibrary {

public:
    MediaLibrary();

    ~MediaLibrary();

    bool add_media(const QString& media_path);
    QVector<Music>& data();
    void setData(int index, Music _new_music);

//    QVector<Music> data();
    Playlist dataPlaylist() const;

private:

    bool load_media_base();

    void add_file(const QString& file_name);
    void add_dir(const QString& dir_name);
     QVector<Music> m_media_list;
};

char *toChar(QString str);

#endif //UAMP_MEDIALIBRARY_H
