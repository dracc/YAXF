#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "settings.h"

#include <QMainWindow>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionConfigure_triggered();

    void on_actionQuit_triggered();

    void on_actionRun_ISO_triggered();

private:
    Ui::MainWindow *ui;
    settings *sett;
};

#endif // MAINWINDOW_H
