#ifndef UTAG_TAG_FUNCTIONS_H
#define UTAG_TAG_FUNCTIONS_H

#include <iostream>
#include <iomanip>
#include <QDir>
#include <QVector>
#include <QString>
#include <string>

#include "../../3dparty/taglib/taglib/toolkit/tpropertymap.h"
#include "../../3dparty/taglib/taglib/fileref.h"
#include "../../3dparty/taglib/taglib/tag.h"
#include "../../3dparty/taglib/taglib/mpeg/id3v2/id3v2frame.h"
#include "../../3dparty/taglib/taglib/mpeg/id3v2/frames/attachedpictureframe.h"
#include "../../3dparty/taglib/taglib/mpeg/mpegfile.h"
#include "../../3dparty/taglib/taglib/mpeg/id3v2/id3v2tag.h"
#include "../../3dparty/taglib/taglib/mpeg/id3v2/frames/unsynchronizedlyricsframe.h"

#include "../../3dparty/taglib/bindings/c/tag_c.h"

class Music;

class ImageFile : public TagLib::File
{
public:
    ImageFile(const char *file) : TagLib::File(file)
    {
    }

    TagLib::ByteVector data()
    {
        return readBlock(length());
    }

private:
    virtual TagLib::Tag *tag() const { return 0; }
    virtual TagLib::AudioProperties *audioProperties() const { return 0; }
    virtual bool save() { return false; }
};

namespace TagFunctions {

    char *toChar(QString str);

    Music read_tags(QString file_path);

    QByteArray load_cover_array(QString file_path);

    QString load_lyrics(const QString& file_path);

    void set_lyrics(const QString &file_path, const QString &lyrics);

    void load_cover(char *file_name);

    QImage load_cover_image(char *file_path);

    QImage load_cover_image_mpeg(char *file_path);

    QImage load_cover_image_m4a(char *file_path);

    QImage load_cover_image_ogg(char *file_path);

    bool set_image_mpeg(const QString& file_path, const QString& image_path);

    void modify_tag_artist(char *file_path, char *new_artist);

    void modify_tag(char *file_path, char field, char *new_value);

    void modify_tag_title(char *file_path, char *new_title);

    void modify_tag_genre(char *file_path, char *new_genre);

    void modify_tag_album(char *file_path, char *new_album);

    void modify_tag_comment(char *file_path, char *new_comment);

    void modify_tag_year(const Music &changes);

    void modify_tag_track(const Music &changes);

    bool modify_tags(const Music &changes);

    Music LoadSongTags(QString file_path);

}


#endif //UTAG_TAG_FUNCTIONS_H
