#ifndef CONFIG_H
#define CONFIG_H

#include "settings.h"
#include "settingsmanager.h"

#include <QDialog>

namespace Ui {
class Config;
}

class Config : public QDialog
{
    Q_OBJECT

public:
    explicit Config(QWidget *parent = 0, settings* sett = nullptr);
    ~Config();

private slots:
    void on_buttonBox_accepted();

    void on_bin_browse_button_clicked();

    void on_xcmp_browse_button_clicked();

    void on_flash_browse_button_clicked();

    void on_hdd_browse_button_clicked();

private:
    settings *sett;
    Ui::Config *ui;
};

#endif // CONFIG_H
