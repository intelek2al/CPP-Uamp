#include <regex>
#include <QImage>
#include <mp4/mp4file.h>
#include <QTime>
#include <QBuffer>
#include <QPixmap>
#include <QStringRef>
#include "tag_functions.h"
#include "loggingcategories.h"
#include "music.h"

#define default_cover ":/def_cover_color.png"

class ImageFile;

/* Unsychronised lyrics/text transcription
 * This frame contains the lyrics of the song or a text transcription of other vocal activities.
 * The head includes an encoding descriptor and a content descriptor. The body consists of the actual text.
 * The 'Content descriptor' is a terminated string. If no descriptor is entered,
 * 'Content descriptor' is $00 (00) only. Newline characters are allowed in the text.
 * There may be more than one 'Unsynchronised lyrics/text transcription' frame in each tag,
 * but only one with the same language and content descriptor.
 *
 * <Header for 'Unsynchronised lyrics/text transcription', ID: "USLT">
    Text encoding       $xx
    Language            $xx xx xx
    Content descriptor  <text string according to encoding> $00 (00)
    Lyrics/text         <full text string according to encoding>
*/


char *TagFunctions::toChar(QString str)
{
    char *test = str.toUtf8().data();
    return test;
}

void TagFunctions::load_lyrics(char *file_name) {
    TagLib::MPEG::File f1(file_name);

    if (f1.ID3v2Tag()) {
        TagLib::ID3v2::FrameList frames = f1.ID3v2Tag()->frameListMap()["USLT"];

        if (!frames.isEmpty()) {
            TagLib::ID3v2::UnsynchronizedLyricsFrame *frame =
                    dynamic_cast<TagLib::ID3v2::UnsynchronizedLyricsFrame *>(frames.front());
            // There could be multiple frames here; you may want to look at language
            // and/or description, instead of just picking the first.
//            if (frame)
//                std::cout << frames.front()->toString() << " here" << std::endl;
        }
    }
}

/*
 *
 * <Header for 'Attached picture', ID: "APIC">
 * Text encoding   $xx
 * MIME type       <text string> $00
 * Picture type    $xx
 * Description     <text string according to encoding> $00 (00)
 * Picture data    <binary data>
 *
 * Picture type:
    $00     Other
    $01     32x32 pixels 'file icon' (PNG only)
    $02     Other file icon
    $03     Cover (front)
    $04     Cover (back)
    $05     Leaflet page
    $06     Media (e.g. lable side of CD)
    $07     Lead artist/lead performer/soloist
    $08     Artist/performer
    $09     Conductor
    $0A     Band/Orchestra
    $0B     Composer
    $0C     Lyricist/text writer
    $0D     Recording Location
    $0E     During recording
    $0F     During performance
    $10     Movie/video screen capture
    $11     A bright coloured fish
    $12     Illustration
    $13     Band/artist logotype
    $14     Publisher/Studio logotype
 */

// deprecated!!!!!!!!!!!!

QImage TagFunctions::load_cover_image(char *file_path) {
    QImage coverQImg(default_cover);

    static const char *IdPicture = "APIC";  //  APIC    [#sec4.15 Attached picture]
    TagLib::MPEG::File mpegFile(file_path);
    TagLib::ID3v2::Tag *id3v2tag = mpegFile.ID3v2Tag();
    TagLib::ID3v2::FrameList Frame;
    TagLib::ID3v2::AttachedPictureFrame *PicFrame;

    if (id3v2tag) {
        Frame = id3v2tag->frameListMap()[IdPicture];
        if (!Frame.isEmpty()) {
            for (TagLib::ID3v2::FrameList::ConstIterator it = Frame.begin(); it != Frame.end(); ++it) {
                PicFrame = static_cast<TagLib::ID3v2::AttachedPictureFrame * >(*it);
                {
//                    if (PicFrame->type() == TagLib::ID3v2::AttachedPictureFrame::FrontCover)
                    if (PicFrame)
                    {
                        coverQImg.loadFromData((const uchar *) PicFrame->picture().data(), PicFrame->picture().size());
                        coverQImg = coverQImg.scaled(190, 190);
                    }
                }
            }
        }
    }
    return coverQImg;
}

unsigned int str_to_uint(const char* new_value) {
    unsigned int res = 0;

    std::regex regular(R"lit(\s*[0-9]+\s*)lit");
    std::cmatch result;
    std::string line = new_value;
    if (std::regex_match(line.c_str(), result, regular)) {
        res = std::stoul(result[0]);

    } else {
        throw std::invalid_argument("invalid argument\n");
    }
    return res;
}


Music TagFunctions::read_tags(QString file_path) {
//      0       1       2           3      4          5       6       7        8       9
//    {"Name", "Time", "Title", "Artist", "Genre", "Album", "Year", "Track", "Path", "Comment" };
    qDebug(logDebug()) << "TagFunctions::read_tags file_path =" << file_path;
    Music data;
//    std::string file_n = file_name;
//    std::string file_p = file_path;
    TagLib::FileRef f(file_path.toStdString().data());

    if (!f.isNull() && f.tag()) {
        TagLib::Tag *tag = f.tag();
//        data.m_name = QString(file_n.data());
        data.m_name = QFileInfo(file_path).fileName();
        data.m_title = tag->title().toCString();
        data.m_artist = tag->artist().toCString();
        data.m_genre = tag->genre().toCString();
        data.m_album = tag->album().toCString();
        data.m_year = QString::number(tag->year());
        data.m_track = QString::number(tag->track());
        data.m_path = file_path;
        data.m_comment =tag->comment().toCString();
        data.m_url = QUrl(data.m_path);

        qDebug(logDebug()) << "TagFunctions::read_tags" << data.m_name;
        qDebug(logDebug()) << "TagFunctions::read_tags" << data.m_path;

        /*
        cout << "-- TAG (basic) --" << endl;
        cout << "Artist  - \"" << tag->artist() << "\"" << endl;
        cout << "Title   - \"" << tag->title() << "\"" << endl;
        cout << "Album   - \"" << tag->album() << "\"" << endl;
        cout << "Genre   - \"" << tag->genre() << "\"" << endl;
        cout << "year    - \"" << tag->year() << "\"" << endl;
        cout << "comment - \"" << tag->comment() << "\"" << endl;
        cout << "track   - \"" << tag->track() << "\"" << endl;
        */

        TagLib::PropertyMap tags = f.file()->properties();

        unsigned int longest = 0;
        for (TagLib::PropertyMap::ConstIterator i = tags.begin(); i != tags.end(); ++i) {
            if (i->first.size() > longest) {
                longest = i->first.size();
            }
        }
        if (!f.isNull() && f.audioProperties()) {
            TagLib::AudioProperties *properties = f.audioProperties();
            int seconds = properties->length() % 60;
            int minutes = (properties->length() - seconds) / 60;
            int hours = (properties->length() - minutes) / 60;
//            QTime time(hours, minutes, seconds);
            QTime time(hours, minutes, seconds);

//            data.m_time =(QString::fromStdString(std::to_string(minutes) +
//                                             ":" + std::to_string(seconds)));
            QString str_seconds = seconds < 9 ? "0" + QString::number(time.second()) : QString::number(time.second());
            data.m_time = QString::number(time.minute()) + ":" + str_seconds;
            /*
            cout << "-- AUDIO --" << endl;
            cout << "bitrate     - " << properties->bitrate() << endl;
            cout << "sample rate - " << properties->sampleRate() << endl;
            cout << "channels    - " << properties->channels() << endl;
            cout << "length      - " << minutes << ":" << std::setfill('0')
                 << std::setw(2) << seconds << endl;
            */
        }
        data.m_cover = load_cover_array(file_path);
    } else {
        qWarning(logWarning()) << "TagFunctions::read_tags  tags doesn't read";
    }
    return data;
}

QByteArray TagFunctions::load_cover_array(QString file_path) {
    if (file_path.isEmpty()) {
        qDebug(logDebug()) << "TagFunctions::load_cover_array file_path EMPTY";
        return QByteArray();
    }
    QByteArray byte_cover;
    QImage coverQImg;

    QString fileName = QFileInfo(file_path).fileName();
    QString fileType = fileName.mid(fileName.lastIndexOf(".") + 1, -1);

//    qDebug(logDebug()) << "file_path = " << file_path;
//    qDebug(logDebug()) << "file type = " << fileType;
    if (fileType == "mp3") {
        coverQImg = TagFunctions::load_cover_image_mpeg(file_path.toStdString().data());
//        coverQImg = TagFunctions::load_cover_image(file_path);
    }
    if (fileType == "m4a") {
        coverQImg = TagFunctions::load_cover_image_m4a(file_path.toStdString().data());
    }

    QBuffer buffer(&byte_cover);
    buffer.open(QIODevice::WriteOnly);
    QPixmap pix_cover(QPixmap::fromImage(coverQImg));
    pix_cover.save(&buffer,"PNG");
    if (byte_cover.isEmpty()) {
        qDebug(logDebug()) << "TagFunctions::load_cover_array empty";
    }
    return byte_cover;
}


void TagFunctions::modify_tag_artist(char *file_path, char *new_artist)
{
    TagLib::FileRef f(file_path);

    if (!f.isNull() && f.tag())
    {
        TagLib::Tag *tag = f.tag();
        tag->setArtist(new_artist);
    }
    f.save();
}


void TagFunctions::modify_tag_title(char *file_path, char *new_title) {
    TagLib::FileRef f(file_path);

    if (!f.isNull() && f.tag())
    {
        TagLib::Tag *tag = f.tag();
        tag->setTitle(new_title);
    }
    f.save();
}

void TagFunctions::modify_tag_genre(char *file_path, char *new_genre) {
    TagLib::FileRef f(file_path);

    if (!f.isNull() && f.tag())
    {
        TagLib::Tag *tag = f.tag();
        tag->setGenre(new_genre);
    }
    f.save();
}

void TagFunctions::modify_tag_album(char *file_path, char *new_album) {
    TagLib::FileRef f(file_path);

    if (!f.isNull() && f.tag())
    {
        TagLib::Tag *tag = f.tag();
        tag->setAlbum(new_album);
    }
    f.save();
}

void TagFunctions::modify_tag_comment(char *file_path, char *new_comment) {
    TagLib::FileRef f(file_path);

    if (!f.isNull() && f.tag())
    {
        TagLib::Tag *tag = f.tag();
        tag->setComment(new_comment);
    }
    f.save();
}

void TagFunctions::modify_tag_year(const Music& changes) {
    TagLib::FileRef f(changes.m_path.toStdString().data());

    unsigned int n_year;
    try {
        n_year = str_to_uint(changes.m_year.toStdString().data());
    }
    catch (std::exception& ex) {
        std::cerr << ex.what();
        return;
    }

    if (!f.isNull() && f.tag())
    {
        TagLib::Tag *tag = f.tag();
        tag->setYear(n_year);
    }
    f.save();
}

void TagFunctions::modify_tag_track(const Music& changes) {
    TagLib::FileRef f(changes.m_path.toStdString().data());
    unsigned int n_track;

    try {
        n_track = str_to_uint(changes.m_track.toStdString().data());
    }
    catch (std::exception& ex) {
        std::cerr << ex.what();
        return;
    }
    if (!f.isNull() && f.tag())
    {
        TagLib::Tag *tag = f.tag();
        tag->setTrack(n_track);
    }
    f.save();
}

bool TagFunctions::modify_tags(const Music& changes) {
    if (QFileInfo file(changes.m_path); !file.isWritable()) {
        return false;
    }

    TagLib::FileRef f(changes.m_path.toStdString().data());

    if (!f.isNull() && f.tag())
    {
        TagLib::Tag *tag = f.tag();
        tag->setArtist(changes.m_artist.toStdString());
        tag->setTitle(changes.m_title.toStdString());
        tag->setGenre(changes.m_genre.toStdString());
        tag->setAlbum(changes.m_album.toStdString());
        tag->setComment(changes.m_comment.toStdString());
    }
    f.save();
    modify_tag_year(changes);
    modify_tag_track(changes);
    return true;
}

QImage TagFunctions::load_cover_image_mpeg(char *file_path)
{
    if (!file_path) {
       return QImage();
    }
    static const char *IdPicture = "APIC";  //  APIC    [#sec4.15 Attached picture]
    TagLib::MPEG::File mpegFile(file_path);
    TagLib::ID3v2::Tag *tag = mpegFile.ID3v2Tag();
    TagLib::ID3v2::FrameList l = tag->frameList("APIC");

    QImage image;

    if (l.isEmpty()) {
        return QImage(default_cover);
    }
    TagLib::ID3v2::AttachedPictureFrame *f =
            static_cast<TagLib::ID3v2::AttachedPictureFrame *>(l.front());

    image.loadFromData((const uchar *) f->picture().data(), f->picture().size());
    return image;
}


QImage TagFunctions::load_cover_image_m4a(char *file_path)
{
    if (!file_path) {
        return QImage();
    }
    QImage image;
    TagLib::MP4::File file(file_path);
    TagLib::MP4::Tag* tag = file.tag();
    TagLib::MP4::ItemListMap itemsListMap = tag->itemListMap();
    TagLib::MP4::Item coverItem = itemsListMap["covr"];
    TagLib::MP4::CoverArtList coverArtList = coverItem.toCoverArtList();

    if (coverArtList.isEmpty()) {
        return QImage(default_cover);
    }
    else
    {
        TagLib::MP4::CoverArt coverArt = coverArtList.front();
        image.loadFromData((const uchar *)
                                   coverArt.data().data(), coverArt.data().size());
        return QImage();
    }
}

QImage TagFunctions::load_cover_image_ogg(char *file_path) {
    return QImage();
}

bool TagFunctions::set_image_mpeg(const QString& file_path, const QString& image_path)
{
    QFileInfo file(file_path);
    if (!file.isWritable()) {
        qInfo(logInfo()) << "file =" << file << "isn't Writable";
        return false;
    }

    qDebug(logDebug()) << "file_path =" << file_path;
    qDebug(logDebug()) << "image_path =" << image_path;

    TagLib::MPEG::File mpegFile(file_path.toStdString().data());
    TagLib::ID3v2::Tag *tag = mpegFile.ID3v2Tag();
    TagLib::ID3v2::FrameList frames = tag->frameList("APIC");
    TagLib::ID3v2::AttachedPictureFrame *frame = 0;

    if(frames.isEmpty())
    {
        qDebug(logDebug()) << "line 423";
        frame = new TagLib::ID3v2::AttachedPictureFrame;
        tag->addFrame(frame);
    } else
    {
        qDebug(logDebug()) << "line 428";
        frame = new TagLib::ID3v2::AttachedPictureFrame;
        frame = static_cast<TagLib::ID3v2::AttachedPictureFrame *>(frames.front());
    }

    ImageFile imageFile(image_path.toStdString().data());
    TagLib::ByteVector imageData = imageFile.data();
    frame->setMimeType("image/jpeg");
    frame->setPicture(imageData);
    mpegFile.save();
    qDebug(logDebug()) << "TagFunctions::set_image_mpeg END";
    return true;
}


Music TagFunctions::LoadSongTags(QString file_path) {

    QFileInfo fileInfo(file_path);
    if (!fileInfo.isReadable()) {
        qWarning(logWarning()) << fileInfo.fileName() << " not readable";
        return Music();
    }
    Music tmp;
    try
    {
        tmp = TagFunctions::read_tags(file_path);
    }
    catch (std::exception &e)
    {
        qWarning(logWarning()) << e.what();
    }
//    std::cout << "MediaLibrary" << tmp.m_path.toStdString() << std::endl;
    return tmp;
}

