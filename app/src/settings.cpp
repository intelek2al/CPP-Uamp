#include <QFile>
#include "settings.h"
#include "app.h"
#include "loggingcategories.h"

//Settings::Settings(const QString& strOrg, const QString& strAppName) {
Settings::Settings() {
//    m_preferences = new QSettings(strOrg, strAppName);
    m_preferences = App::get_app()->app_settings();

  // load saved theme
    QString theme = m_preferences->value("theme").toString();
    loadTheme(theme);
    m_reverse_timer = new QTimer(0);
}

Settings::~Settings() {
//    delete m_preferences;
    delete m_reverse_timer;
}


void Settings::set_settings(QMap<QString, QString> _settings) {

      if (current_settings["theme"] != _settings["theme"]) {
          current_settings["theme"] = _settings["theme"];
          loadTheme(_settings["theme"]);
          qInfo(logInfo()) << "new theme =" << _settings["theme"];
          m_preferences->setValue("theme", current_settings["theme"]);
      }
}

void Settings::loadTheme(QString &theme) {
    if (!theme.isEmpty()) {
        QFile file(":/qss/" + theme + ".qss");
        file.open(QFile::ReadOnly);
        App::get_app()->setStyleSheet(file.readAll());
        current_settings["theme"] = theme;
    }
    qInfo(logInfo()) << theme;
}

QMap<QString, QString> Settings::get_current_settings() const {
    return current_settings;
}

void Settings::setTimer(int msec) {
    m_reverse_timer->start(msec);
}

void Settings::stopTimer() {
    m_reverse_timer->stop();
}

QTimer *Settings::getTimer() const {
    return m_reverse_timer;
}
