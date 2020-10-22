#ifndef UTAG_LOGGER_H
#define UTAG_LOGGER_H

#include <QString>
#include <QVector>
#include <QDate>
#include <QTextStream>

namespace Ui {
    class MainWindow;
}

class Logger {
public:
    Logger(Ui::MainWindow *child);

    virtual ~Logger();

    void add_log_massage(QString message);

    void show_logger() const;

private:
    Ui::MainWindow *ui;
    QString m_logger;
};

#endif //UTAG_LOGGER_H
