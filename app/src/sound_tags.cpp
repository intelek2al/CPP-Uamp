//    {"Name", "Time", "Title", "Artist", "Genre", "Album", "Year", "Track", "Path", "Comment" };

#include "sound_tags.h"

#include <utility>

using std::cout, std::endl;

Sound_tags::Sound_tags() {
    sound_file = new QMediaPlayer;
}

void Sound_tags::read_tags(QString file, QString file_path) {

    std::cout << "file = " << file.toStdString() << std ::endl;
    std::cout << "file path= " << file_path.toStdString() << std ::endl;

    sound_file->setMedia(QUrl::fromLocalFile(file_path));


//    sound_file->setMedia(QUrl::fromLocalFile(file_path));
//
//    while(!sound_file->isMetaDataAvailable()){
//        QCoreApplication::processEvents();
//    }
    if (sound_file->isMetaDataAvailable()) {
//        ui->statusbar->showMessage("Audio Avalib ", 2000);
        cout << file.toStdString() << " loaded" << endl;
//        ui->statusbar->showMessage("Audio Avalib ", 2000);
    }
    else {
        while (!sound_file->isMetaDataAvailable()) {
            QCoreApplication::processEvents();
        }
    }

    tags[0] = file;
    tags[1] = "00 :01";
    tags[2] = sound_file->metaData(QStringLiteral("Title")).toString();
    tags[3] = sound_file->metaData(QStringLiteral("ContributingArtist")).toString();
    tags[4] = sound_file->metaData(QStringLiteral("Genre")).toString();
    tags[5] = sound_file->metaData(QStringLiteral("AlbumTitle")).toString();
    tags[6] = sound_file->metaData(QStringLiteral("Year")).toString();
    tags[7] = sound_file->metaData(QStringLiteral("TrackNumber")).toString();
    tags[8] = file_path;
    tags[9] = sound_file->metaData(QStringLiteral("Comment")).toString();


    cout << "\n print all tags ========\n" <<
     "title\t\t" << sound_file->metaData(QStringLiteral("Title")).toString().toStdString() << endl <<
     " Year\t\t"  <<
    sound_file->metaData(QStringLiteral("Year")).toString().toStdString() << std::endl

   <<  "\n print all tags ========" << endl;

}

Sound_tags::~Sound_tags() {
//    std::cout << "destructor Sound_tags" << std::endl;
    delete sound_file;
}

QVector<QString> Sound_tags::get_vector() const {
    return tags;
}

