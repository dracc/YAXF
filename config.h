#ifndef CONFIG_H
#define CONFIG_H

#include "settings.h"
#include "settingsmanager.h"

extern "C"{
#include <libusb-1.0/libusb.h>
}

#include <QDialog>

namespace Ui {
class Config;
}

class Config : public QDialog
{
    Q_OBJECT

public:
    explicit Config(QWidget *parent = 0, settings* sett = nullptr,
                    QVector<libusb_device *> *controllers = nullptr);
    ~Config();

private slots:
    void on_buttonBox_accepted();

    void on_bin_browse_button_clicked();

    void on_mcpx_browse_button_clicked();

    void on_flash_browse_button_clicked();

    void on_hdd_browse_button_clicked();

    void on_controller_select_1_currentIndexChanged(int index);

    void on_xiso_browse_button_clicked();

private:
    settings *sett;
    QVector<libusb_device *> *controllers;
    Ui::Config *ui;
};

#endif // CONFIG_H
