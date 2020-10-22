#ifndef UTEXT_SETTINGS_H
#define UTEXT_SETTINGS_H

#include <QSettings>
#include <Qmap>
#include <QStringList>


QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE

class Settings {

public:
//    Settings(const QString& strOrg, const QString& strAppName);

    Settings();  //
    virtual ~Settings();
    void set_settings(QMap<QString, QString> _settings);
    void loadTheme(QString& theme);
    QMap<QString, QString> get_current_settings() const;

private:
    QSettings *m_preferences;
    QMap<QString, QString> current_settings;  // {{"font", "Arial"}, {"theme", "Dark"}}
};


#endif //UTEXT_SETTINGS_H
