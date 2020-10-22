#ifndef DIALOGPREFERENCES_H
#define DIALOGPREFERENCES_H

#include <QDialog>
#include <QMap>
#include <QStringList>

namespace Ui {
class DialogPreferences;
}

class DialogPreferences : public QDialog
{
    Q_OBJECT

public:
    explicit DialogPreferences(QMap<QString, QString> _settings, QWidget *parent = nullptr);
    ~DialogPreferences();

    void accepted();

    void get_dialog_options(QMap<QString, QString>& dialog_options) const;

signals:
    QMap<QString, QString> SavedSettings(QMap<QString, QString>);

private:
    Ui::DialogPreferences *ui;
    QMap<QString, QString> m_dialog_settings;
};

#endif // DIALOGPREFERENCES_H
