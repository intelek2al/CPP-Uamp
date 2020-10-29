#ifndef MUSIC_H
#define MUSIC_H

#include <QObject>
#include <QUrl>
#include <QVariant>

class Music : public QObject
{
public:
    explicit Music(QObject *parent = nullptr);
    Music(const Music &m);
    Music(const QUrl &url);
    explicit Music(const QVector<QString> &m);
    explicit Music(QVector<QString> &&m);
    Music(Music &&m) noexcept;
    ~Music()= default;
    Music& operator=(QVector<QString> &&m) noexcept;
    Music& operator=(const QVector<QString> &m);
    Music& operator=(Music &&m) noexcept;
    Music& operator=(const Music &m);
    QString &operator[](int i);
    QString operator[](int i) const;
    QVector<QString> getMusicInfo() const;
    bool operator==(const Music &m) const;
    bool empty() const;

    //    {"Name", "Time", "Title", "Artist", "Rating", "Genre", "Album", "Year", "Track", "Comment", "Path" };

    QString m_name;
    QString m_time;
    QString m_title;
    QString m_artist;
    QString m_rate;
    QString m_genre;
    QString m_album;
    QString m_year;
    QString m_track;
    QString m_comment;
    QString m_path;
    QString m_lyrics;
    quint8 m_count;
    QUrl m_url;
};

#endif