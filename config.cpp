#include "config.h"
#include "ui_config.h"

#include <QFileDialog>
#include <string>
#include <QPair>
#include <QVector>

Config::Config(QWidget *parent, settings *sett,
               QVector<libusb_device *> *controllers, int tabselect) :
    QDialog(parent),
    ui(new Ui::Config),
    sett(sett),
    controllers(controllers)

{
    ui->setupUi(this);
    ui->tabWidget->setCurrentIndex(tabselect);
    ui->bin_input_box->setText(sett->bin_path);
    ui->mcpx_input_box->setText(sett->mcpx_path);
    ui->flash_input_box->setText(sett->flash_path);
    ui->hdd_input_box->setText(sett->hdd_path);
    ui->xiso_input_box->setText(sett->xiso_path);
    ui->expand_ram->setChecked(sett->expanded_ram);
    ui->full_boot_anim->setChecked(sett->full_boot_anim);
    ui->hdd_unlocked->setChecked(sett->hdd_unlocked);
    ui->controller_bool_1->setChecked(sett->c_1_plugged);
    ui->controller_bool_2->setChecked(sett->c_2_plugged);
    ui->controller_bool_3->setChecked(sett->c_3_plugged);
    ui->controller_bool_4->setChecked(sett->c_4_plugged);
    ui->enable_kvm->setChecked(sett->cpuaccel);
    ui->enable_gl->setChecked(sett->sdl_gl);

    libusb_device_descriptor desc;
    for(int q(0); q < controllers->size(); ++q){
        libusb_device *p = controllers->at(q);
        desc = {0};
        libusb_get_device_descriptor(p, &desc);
        QString DeviceName;
        if(desc.idProduct == 0x202){
            DeviceName = "Xbox Duke";
        }
        else if(desc.idProduct == 0x289){
            DeviceName = "Xbox Controller S";
        }
        else if(desc.idProduct == 0xca8a){
            DeviceName = "Logitech Wheel";
        }
        ui->controller_select_1->addItem(DeviceName, q);
        ui->controller_select_2->addItem(DeviceName, q);
        ui->controller_select_3->addItem(DeviceName, q);
        ui->controller_select_4->addItem(DeviceName, q);
    }

    ui->controller_select_1->setCurrentIndex(sett->ctrl_1);
    ui->controller_select_2->setCurrentIndex(sett->ctrl_2);
    ui->controller_select_3->setCurrentIndex(sett->ctrl_3);
    ui->controller_select_4->setCurrentIndex(sett->ctrl_4);

    args = settingsManager::genArgs(sett, "<.iso>", *controllers);
}

Config::~Config()
{
    delete ui;
}

template<typename T>
void checkSetValue(T &stored_val, T const check_val, QString const& iniVal){
    if(stored_val != check_val){
        stored_val = check_val;
        settingsManager::storeSetting(iniVal, stored_val);
    }
}


void Config::on_buttonBox_accepted()
{
    checkSetValue(sett->expanded_ram, ui->expand_ram->isChecked(), "expandedRAM");
    checkSetValue(sett->full_boot_anim, ui->full_boot_anim->isChecked(), "fullBootAnim");
    checkSetValue(sett->hdd_unlocked, ui->hdd_unlocked->isChecked(), "hddUnlocked");
    checkSetValue(sett->c_1_plugged, ui->controller_bool_1->isChecked(), "C1Plugged");
    checkSetValue(sett->c_2_plugged, ui->controller_bool_2->isChecked(), "C2Plugged");
    checkSetValue(sett->c_3_plugged, ui->controller_bool_3->isChecked(), "C3Plugged");
    checkSetValue(sett->c_4_plugged, ui->controller_bool_4->isChecked(), "C4Plugged");
    checkSetValue(sett->cpuaccel, ui->enable_kvm->isChecked(), "enableCPUaccel");
    checkSetValue(sett->sdl_gl, ui->enable_gl->isChecked(), "enableSDLGL");
    checkSetValue(sett->ctrl_1, ui->controller_select_1->currentIndex(), "ctrl_1");
    checkSetValue(sett->ctrl_2, ui->controller_select_2->currentIndex(), "ctrl_2");
    checkSetValue(sett->ctrl_3, ui->controller_select_3->currentIndex(), "ctrl_3");
    checkSetValue(sett->ctrl_4, ui->controller_select_4->currentIndex(), "ctrl_4");
}

void Config::on_bin_browse_button_clicked()
{
    QString tmp = QFileDialog::getOpenFileName(this, 0,
					       sett->bin_path);
    if(tmp != ""){
        sett->bin_path = tmp;
        ui->bin_input_box->setText(sett->bin_path);
        settingsManager::storeSetting("binPath", tmp);
    }
}

void Config::on_mcpx_browse_button_clicked()
{
    QString tmp = QFileDialog::getOpenFileName(this, 0, sett->mcpx_path,
					       "Binary files (*.bin);;All files (*.*)");
    if(tmp != ""){
        sett->mcpx_path = tmp;
        ui->mcpx_input_box->setText(sett->mcpx_path);
        settingsManager::storeSetting("mcpxPath", tmp);
    }
}

void Config::on_flash_browse_button_clicked()
{
    QString tmp = QFileDialog::getOpenFileName(this, 0, sett->flash_path,
					       "Binary files (*.bin);;All files (*.*)");
    if(tmp != ""){
        sett->flash_path = tmp;
        ui->flash_input_box->setText(sett->flash_path);
        settingsManager::storeSetting("flashPath", tmp);
    }
}

void Config::on_hdd_browse_button_clicked()
{
    QString tmp = QFileDialog::getOpenFileName(this, 0, sett->hdd_path,
					       "QCow2 files (*.qcow2);;All files (*.*)");
    if(tmp != ""){
        sett->hdd_path = tmp;
        ui->hdd_input_box->setText(sett->hdd_path);
        settingsManager::storeSetting("hddPath", tmp);
    }
}

void Config::on_xiso_browse_button_clicked()
{
  QString tmp = QFileDialog::getExistingDirectory(this, "", sett->xiso_path);
    if(tmp != ""){
        sett->xiso_path = tmp;
        ui->xiso_input_box->setText(sett->xiso_path);
        settingsManager::storeSetting("xisoPath", tmp);
    }
}

void Config::on_extraArguments_textChanged()
{
    QString extras = ui->extraArguments->toPlainText();
    QStringList extraArgs = extras.split(" ");
    ui->currentArguments->setPlainText(sett->bin_path
                                       + " " +  args.join(" ")
                                       + " " + extraArgs.join(" "));
}

void Config::on_tabWidget_currentChanged(int index)
{
    if(index == 4){
        args = settingsManager::genArgs(sett, "<.iso>", *controllers);
        on_extraArguments_textChanged();
    }
}
