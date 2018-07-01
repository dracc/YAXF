#include "config.h"
#include "ui_config.h"

#include <QFileDialog>
#include <string>
#include <QPair>
#include <QVector>

extern "C" {
#include <libusb-1.0/libusb.h>
}

Config::Config(QWidget *parent, settings *sett,
               QVector<QPair<int,int>> *controllers) :
    QDialog(parent),
    sett(sett),
    controllers(controllers),
    ui(new Ui::Config)
{
    ui->setupUi(this);
    ui->bin_input_box->setText(sett->bin_path);
    ui->mcpx_input_box->setText(sett->mcpx_path);
    ui->flash_input_box->setText(sett->flash_path);
    ui->hdd_input_box->setText(sett->hdd_path);
    ui->expand_ram->setChecked(sett->expanded_ram);
    ui->full_boot_anim->setChecked(sett->full_boot_anim);
    ui->hdd_unlocked->setChecked(sett->hdd_unlocked);

    for(int q(0); q < controllers->size(); ++q){
        ui->controller_select_1->addItem(QString("asd"), q);
    }

    ui->controller_select_1->setCurrentIndex(sett->ctrl_1);
}

Config::~Config()
{
    delete ui;
}

void checkSetBool(bool &stored_val, bool const check_val, QString const& iniVal){
    if(stored_val != check_val){
        stored_val = !stored_val;
        storeSetting(iniVal, stored_val);
    }
}


void Config::on_buttonBox_accepted()
{
    checkSetBool(sett->expanded_ram, ui->expand_ram->isChecked(), "expandedRAM");
    checkSetBool(sett->full_boot_anim, ui->full_boot_anim->isChecked(), "fullBootAnim");
    checkSetBool(sett->hdd_unlocked, ui->hdd_unlocked->isChecked(), "hddUnlocked");
}

void Config::on_bin_browse_button_clicked()
{
    QString tmp = QFileDialog::getOpenFileName(this);
    if(tmp != ""){
        sett->bin_path = tmp;
        ui->bin_input_box->setText(sett->bin_path);
        storeSetting("binPath", tmp);
    }
}

void Config::on_xcmp_browse_button_clicked()
{
    QString tmp = QFileDialog::getOpenFileName(this, 0, "", "Binary files (*.bin);;All files (*.*)");
    if(tmp != ""){
        sett->mcpx_path = tmp;
        ui->mcpx_input_box->setText(sett->mcpx_path);
        storeSetting("xcmpPath", tmp);
    }
}

void Config::on_flash_browse_button_clicked()
{
    QString tmp = QFileDialog::getOpenFileName(this, 0, "", "Binary files (*.bin);;All files (*.*)");
    if(tmp != ""){
        sett->flash_path = tmp;
        ui->flash_input_box->setText(sett->flash_path);
        storeSetting("flashPath", tmp);
    }
}

void Config::on_hdd_browse_button_clicked()
{
    QString tmp = QFileDialog::getOpenFileName(this, 0, "", "QCow2 files (*.qcow2);;All files (*.*)");
    if(tmp != ""){
        sett->hdd_path = tmp;
        ui->hdd_input_box->setText(sett->hdd_path);
        storeSetting("hddPath", tmp);
    }
}

void Config::on_controller_select_1_currentIndexChanged(int index)
{
    sett->ctrl_1 = index;
    storeSetting("ctrl_1", index);
}
