#ifndef UTAG_SOUND_TAGS_H
#define UTAG_SOUND_TAGS_H

#include <QCoreApplication>
#include <QMediaPlayer>
#include <QtMultimedia>
#include <iostream>

class Sound_tags {
public:
    Sound_tags();
    ~Sound_tags();
    void read_tags(QString file, QString file_path);
    QVector <QString> get_vector() const;
    void print_tags() const;

private:
    QMediaPlayer *sound_file;
    QVector<QString> tags{10};
};


#endif //UTAG_SOUND_TAGS_H
