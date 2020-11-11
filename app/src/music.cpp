#include "music.h"
#include <tuple>
#include "tag_functions.h"

Music::Music(QObject *parent) : QObject(parent)
{
    m_url = QUrl();
}

Music::Music(const QUrl &url) {
    m_url = url;
    std::string file_n= url.fileName().toStdString();
    std::string file_p= url.path().toStdString();
    TagLib::FileRef f(file_p.data());

    if (!f.isNull() && f.tag()) {
        TagLib::Tag *tag = f.tag();
        m_name = QString(file_n.data());
        m_title = tag->title().toCString();
        m_artist = tag->artist().toCString();
        m_genre = tag->genre().toCString();
        m_album = tag->album().toCString();
        m_year = QString::number(tag->year());
        m_track = QString::number(tag->track());
        m_path = QString(file_p.data());
        m_comment = tag->comment().toCString();
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
            m_time =(QString::fromStdString(std::to_string(minutes) +
                                                 ":" + std::to_string(seconds)));
        }
        m_lyrics = TagFunctions::load_lyrics(file_p.data());
        m_cover = TagFunctions::load_cover_array(file_p.data());
    }
}


bool Music::operator==(const Music &m) const {
    return std::tie(m_name, m_time, m_title, m_artist, m_genre, m_album,
                    m_path, m_lyrics, m_comment, m_count, m_rate, m_year, m_track, m_cover)
                    == std::tie(m.m_name, m.m_time, m.m_title, m.m_artist, m.m_genre, m.m_album,
                           m.m_path, m.m_lyrics, m.m_comment, m.m_count, m.m_rate, m.m_year, m.m_track, m.m_cover);
//    return m_path == m.m_path;
}

Music::Music(const Music &m) {
    m_name = m.m_name;
    m_time = m.m_time;
    m_title = m.m_title;
    m_artist = m.m_artist;
    m_genre = m.m_genre;
    m_album = m.m_album;
    m_path = m.m_path;
    m_lyrics = m.m_lyrics;
    m_comment = m.m_comment;
    m_count = m.m_count;
    m_rate = m.m_rate;
    m_year = m.m_year;
    m_track = m.m_track;
    m_cover = m.m_cover;
    m_count = m.m_count;
    m_url = QUrl::fromLocalFile(m_path);
}

Music::Music(Music &&m) noexcept {
    m_name = std::move(m.m_name);
    m_time = std::move(m.m_time);
    m_title = std::move(m.m_title);
    m_artist = std::move(m.m_artist);
    m_genre = std::move(m.m_genre);
    m_album = std::move(m.m_album);
    m_path = std::move(m.m_path);
    m_lyrics = std::move(m.m_lyrics);
    m_comment = std::move(m.m_comment);
    m_count = m.m_count;
    m_rate = m.m_rate;
    m_year = m.m_year;
    m_track = m.m_track;
    m_cover = m.m_cover;
    m_count = m.m_count;
    m_url = QUrl::fromLocalFile(m_path);
}

Music& Music::operator=(Music &&m) noexcept {
    m_name = std::move(m.m_name);
    m_time = std::move(m.m_time);
    m_title = std::move(m.m_title);
    m_artist = std::move(m.m_artist);
    m_genre = std::move(m.m_genre);
    m_album = std::move(m.m_album);
    m_path = std::move(m.m_path);
    m_lyrics = std::move(m.m_lyrics);
    m_comment = std::move(m.m_comment);
    m_count = m.m_count;
    m_rate = m.m_rate;
    m_year = m.m_year;
    m_track = m.m_track;
    m_cover = m.m_cover;
    m_count = m.m_count;
    m_url = QUrl::fromLocalFile(m_path);
    return *this;
}

Music& Music::operator=(const Music &m) {
    m_name = m.m_name;
    m_time = m.m_time;
    m_title = m.m_title;
    m_artist = m.m_artist;
    m_genre = m.m_genre;
    m_album = m.m_album;
    m_path = m.m_path;
    m_lyrics = m.m_lyrics;
    m_comment = m.m_comment;
    m_count = m.m_count;
    m_rate = m.m_rate;
    m_year = m.m_year;
    m_track = m.m_track;
    m_cover = m.m_cover;
    m_count = m.m_count;
    m_url = QUrl::fromLocalFile(m_path);
    return *this;
}

//{"Title", "Time", "Artist", "Rating", "Genre", "Album", "Year", "Track", "Comment", "Name","Path"};

QString Music::operator[](int i) const {
    switch (i) {
        case 0: return m_title;
        case 1: return m_time;
        case 2: return m_artist;
        case 3: return m_rate;
        case 4: return m_genre;
//        case 9: return m_rate;
        case 5: return m_album;
        case 6: return m_year;
        case 7: return m_track;
        case 8: return m_comment;
        case 9: return m_name;
        case 10: return m_path;
        case 11: return m_lyrics;

//        case 12: return QString::number(static_cast<int>(m_count));
        default: return m_name;
    }
}

QString &Music::operator[](int i) {
    switch (i) {
        case 0: return m_title;
        case 1: return m_time;
        case 2: return m_artist;
        case 3: return m_rate;
        case 4: return m_genre;
//        case 9: return m_rate;
        case 5: return m_album;
        case 6: return m_year;
        case 7: return m_track;
        case 8: return m_comment;
        case 9: return m_name;
        case 10: return m_path;
        case 11: return m_lyrics;
//        case 12: return m_count;
        default: return m_name;
    }
}



bool Music::empty() const {
    return m_url.isEmpty();
}

QString Music::getStr() const {
    QString temp = "name =" + m_name + "  time ="  + m_time + " title =" + m_title +
            " path =" + m_path;
    return temp;
}




/*
QVector<QString> Music::getMusicInfo() const {
    QVector<QString> info;
    info.push_back(m_name);
    info.push_back(m_time);
    info.push_back(m_title);
    info.push_back(m_artist);
    info.push_back(m_genre);
    info.push_back(m_album);
    info.push_back(m_path);
    info.push_back(m_lyrics);
    info.push_back(m_comment);
    info.push_back(m_rate);
    info.push_back(m_year);
    info.push_back(m_track);
    info.push_back(QString::number(static_cast<int>(m_count)));
    return info;
}

 * Music::Music(const QVector<QString> &m) {  // depracated
    m_name = m[0];
    m_time = m[1];
    m_title = m[2];
    m_artist = m[3];
    m_genre = m[4];
    m_album = m[5];
    m_path = m[6];
    m_lyrics = m[7];
    m_comment = m[8];
    m_count = 0;
    m_rate = m[9].toUInt();
    m_year = static_cast<short>(m[10].toInt());
    m_track = static_cast<short>(m[11].toInt());
    m_url = QUrl::fromLocalFile(m_path);
}


Music::Music(QVector<QString> &&m) {
    m_name = std::move(m[0]);
    m_time = std::move(m[1]);
    m_title = std::move(m[2]);
    m_artist = std::move(m[3]);
    m_genre = std::move(m[4]);
    m_album = std::move(m[5]);
    m_path = std::move(m[6]);
    m_lyrics = std::move(m[7]);
    m_comment = std::move(m[8]);
    m_count = 0;
    m_rate = m[9].toUInt();
    m_year = static_cast<short>(m[10].toInt());
    m_track = static_cast<short>(m[11].toInt());
    m_url = QUrl::fromLocalFile(m_path);
    m.clear();
}

Music& Music::operator=(QVector<QString> &&m) noexcept {
    m_name = std::move(m[0]);
    m_time = std::move(m[1]);
    m_title = std::move(m[2]);
    m_artist = std::move(m[3]);
    m_genre = std::move(m[4]);
    m_album = std::move(m[5]);
    m_path = std::move(m[6]);
    m_lyrics = std::move(m[7]);
    m_comment = std::move(m[8]);
    m_count = 0;
    m_rate = m[9].toUInt();
    m_year = static_cast<short>(m[10].toInt());
    m_track = static_cast<short>(m[11].toInt());
    m_url = QUrl::fromLocalFile(m_path);
    m.clear();
    return *this;
}

Music& Music::operator=(const QVector<QString> &m) {
    m_name = m[0];
    m_time = m[1];
    m_title = m[2];
    m_artist = m[3];
    m_genre = m[4];
    m_album = m[5];
    m_path = m[6];
    m_lyrics = m[7];
    m_comment = m[8];
    m_count = 0;
    m_rate = m[9].toUInt();
    m_year = static_cast<short>(m[10].toInt());
    m_track = static_cast<short>(m[11].toInt());
    m_url = QUrl::fromLocalFile(m_path);
    return *this;
}
 */