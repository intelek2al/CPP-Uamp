#include <QFile>
#include <iostream>
#include "logger.h"
#include "ui_mainwindow.h"

Logger::Logger(Ui::MainWindow *child) {
    ui = child;
}

Logger::~Logger() {
}

void Logger::show_logger() const {
    ui->log->setText(m_logger);
}

void Logger::add_log_massage(QString message) {
    m_logger += QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz ")
                + " " + message + "\n";
    show_logger();
}
