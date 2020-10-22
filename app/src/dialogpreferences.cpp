#include "dialogpreferences.h"
#include "ui_dialogpreferences.h"
#include "loggingcategories.h"

DialogPreferences::DialogPreferences(QMap<QString, QString> _settings, QWidget *parent) :
        QDialog(parent),
        ui(new Ui::DialogPreferences)
{
    ui->setupUi(this);

    m_dialog_settings = _settings;
    QStringList themes({"Default", "ConsoleStyle", "ElegantDark",
                        "MaterialDark", "DarkSolarized", "ManjaroMix"});
    ui->themeComboBox->insertItems(0, themes);
    if (int index = themes.indexOf(m_dialog_settings["theme"]); index > 0) {
        ui->themeComboBox->setCurrentIndex(index);
    } else {
        ui->themeComboBox->setCurrentIndex(0);
    }
}

DialogPreferences::~DialogPreferences()
{
    delete ui;
}

void DialogPreferences::accepted() {
    qDebug(logDebug()) << "ui->themeComboBox->currentData()" << ui->themeComboBox->currentData();
    qDebug(logDebug()) << "ui->themeComboBox->currentText()" << ui->themeComboBox->currentText();
    m_dialog_settings["theme"] = ui->themeComboBox->currentText();

    emit SavedSettings(m_dialog_settings);
    QDialog::accept();
}
void DialogPreferences::get_dialog_options(QMap<QString, QString> &dialog_options) const {
    dialog_options["theme"] = ui->themeComboBox->currentText();
}
