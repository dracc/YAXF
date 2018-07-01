#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "settings.h"

#include <QMainWindow>

extern "C" {
#include <libusb-1.0/libusb.h>
}

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QVector<QPair<int,int>> xbox_controllers;

private slots:
    void on_actionConfigure_triggered();

    void on_actionQuit_triggered();

    void on_actionRun_ISO_triggered();

private:
    Ui::MainWindow *ui;
    settings *sett;

    libusb_context *context = NULL;
    libusb_device **device_list = NULL;
    libusb_device *iterator = NULL;
    libusb_device_descriptor desc = {0};
    int num_devices;
};

#endif // MAINWINDOW_H
