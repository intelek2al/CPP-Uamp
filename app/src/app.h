#ifndef UTEXT_APP_H
#define UTEXT_APP_H

#include <QApplication>
#include <QSettings>

/*
    app settings
 * А обращение к объекту настроек из любой библиотеки проекта будет таким:
    QSettings* pst = App::theApp()->settings();
    pst->setV alue ("Language", "en");
 */

class App : public QApplication {

    Q_OBJECT

public:
    App (int& argc, char** argv, const QString& strOrg, const QString& strAppName);
    static App* get_app();
    QSettings *app_settings();

private:
    QSettings *m_utext_settings;

};

#endif //UTEXT_APP_H
