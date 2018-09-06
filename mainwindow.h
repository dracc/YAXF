#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "settings.h"

#include <QMainWindow>
#include <QDirIterator>
#include <QListWidgetItem>
#include <QString>

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

    QVector<libusb_device *> xbox_controllers;

private slots:
    void on_actionConfigure_triggered();

    void on_actionQuit_triggered();

    void on_actionRun_ISO_triggered();

    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

    void on_actionControls_triggered();

    void on_actionPaths_triggered();

    void on_actionNetwork_triggered();

private:
    Ui::MainWindow *ui;
    settings *sett;

    libusb_context *context = NULL;
    libusb_device **device_list = NULL;
    libusb_device *iterator = NULL;
    libusb_device_descriptor desc = {0};
    int num_devices;

    void openConfigWindow(int tab = 0);
    void RunGame(QString const& path);
};

#endif // MAINWINDOW_H
