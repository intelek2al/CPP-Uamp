#ifndef UTEXT_SETTINGS_H
#define UTEXT_SETTINGS_H

#include <QSettings>
#include <Qmap>
#include <QStringList>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}

QT_END_NAMESPACE

class Settings {

public:
    Settings();  //
    virtual ~Settings();
    void set_settings(QMap<QString, QString> _settings);
    void loadTheme(QString& theme);
    QMap<QString, QString> get_current_settings() const;
    QTimer* getTimer() const;

public slots:
    void setTimer(int msec);
    void stopTimer();

private:
    QSettings *m_preferences;
    QMap<QString, QString> current_settings;  // {{"font", "Arial"}, {"theme", "Dark"}}
    QTimer *m_reverse_timer;
};


#endif //UTEXT_SETTINGS_H
