#include "medialibrary.h"
#include "loggingcategories.h"
#include "tag_functions.h"
#include <QDir>

MediaLibrary::MediaLibrary() {
    // load from base
    if (!load_media_base()) {
        m_media_list = {};
    }
}

bool MediaLibrary::load_media_base() {
    return false;
}

MediaLibrary::~MediaLibrary() {
    // save changes
    // close media base
}

bool MediaLibrary::add_media(const QString &media_path) {
    if (QDir cur_dir(media_path); !cur_dir.isReadable())
    {
        qWarning(logWarning()) << QString(QDir(media_path).dirName() + " directory not readable");
    }
    else {
        add_dir(media_path);
    }
    return false;
}

void MediaLibrary::add_dir(const QString &dir_name) {
    QDir cur_dir(dir_name);

    cur_dir.setFilter(QDir::NoDotAndDotDot | QDir::Files);
    cur_dir.setNameFilters(QStringList() << "*.mp3"
                                                   << "*.flac"
                                                   << "*.wav"
                                                   << "*.ogg"
                                                   << "*.m4a"
                                                   << "*.aif");
    //    current_directory.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    //    current_directory.setSorting(QDir::Size | QDir::Reversed);

    QFileInfoList list = cur_dir.entryInfoList();
    for (int i = 0; i < list.size(); ++i)
    {
        add_file(list.at(i).filePath());
    }
    qInfo(logInfo()) << "m_media_list size =" << m_media_list.size();
}

void MediaLibrary::add_file(const QString &file_name) {

    QFileInfo fileInfo(file_name);
    if (!fileInfo.isReadable()) {
        qWarning(logWarning()) << fileInfo.fileName() << " not readable";
        return;
    }

    Music tmp;
    try
    {
//            Sound_tags current;
//        tmp = Music(QUrl::fromLocalFile(QString(fileInfo.filePath())));
        tmp = TagFunctions::read_tags(file_name);
    }
    catch (std::exception &e)
    {
        std::cerr << "QQQQQQQQQ " << e.what() << std::endl;
    }
    if (!tmp.empty())
        m_media_list.push_back(tmp);
}

QVector<Music>& MediaLibrary::data() {
    return m_media_list;
}

void MediaLibrary::setData(int index, Music _new_music) {
    m_media_list[index] = _new_music;
}

Playlist MediaLibrary::dataPlaylist() const {
    Playlist list;
//    cout << "!" << endl;
    for (const auto &music : m_media_list) {
//        cout << "---- " << music.getStr().toStdString() << endl;
        list.addMusic(music);
    }
    std::cout << endl;
    return list;
}

