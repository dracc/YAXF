#include "config.h"
#include "mainwindow.h"
#include "settings.h"
#include "settingsmanager.h"
#include "ui_mainwindow.h"

#include <iostream>
#include <QProcess>
#include <QFileDialog>
#include <QPair>
#include <QVector>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    sett = new settings;
    initSettings(sett);
    int bus_number, dev_addr;

    int rc = libusb_init(&context);
    num_devices = libusb_get_device_list(context, &device_list);
    for(int i = 0; i < num_devices; ++i){
        iterator = device_list[i];
        desc = {0};
        rc = libusb_get_device_descriptor(iterator, &desc);
        bus_number = libusb_get_bus_number(iterator);
        dev_addr = libusb_get_device_address(iterator);
        if(desc.idVendor == 0x045e){
            if(desc.idProduct == 0x202 | desc.idProduct == 0x289){
                xbox_controllers.push_back(iterator);
            }
        }
    }
    QDirIterator qdit(sett->xiso_path, QStringList() << "*.iso", QDir::Files, QDirIterator::Subdirectories);
    while(qdit.hasNext()){
        QListWidgetItem *q = new QListWidgetItem(QIcon(":/dvd.svg"), qdit.next());
        ui->listWidget->addItem(q);
    }
}

MainWindow::~MainWindow()
{
    libusb_free_device_list(device_list, num_devices);
    libusb_exit(context);
    delete ui;
}

void MainWindow::openConfigWindow(int tab){
    Config* c = new Config(this, sett, &xbox_controllers, tab);
    c->show();
}

void MainWindow::on_actionConfigure_triggered()
{
    openConfigWindow();
}

void MainWindow::on_actionQuit_triggered()
{
    this->close();
}

void MainWindow::RunGame(QString const& path){
    QProcess process;
    QStringList args;
    args << "-cpu" << "pentium3";
    args << "-machine" << "xbox,bootrom=" + sett->mcpx_path + (sett->full_boot_anim ? "":",short_animation");
    args << QString("-m") << QString(sett->expanded_ram ? "128":"64");
    args << "-bios" << sett->flash_path;
    args << "-drive" << "file=" + sett->hdd_path + ",index=0,media=disk" + (sett->hdd_unlocked ? "" : ",locked");
    args << "-drive" << "file=" + path + ",index=1,media=cdrom";
    if(sett->ctrl_1 >= 2){
        int hostbus = libusb_get_bus_number(xbox_controllers.at(sett->ctrl_1 - 2));
        int hostaddr = libusb_get_device_address(xbox_controllers.at(sett->ctrl_1 - 2));
        args << /*"-usb" << */"-device"
             << "usb-host,port=3,hostbus=" + QString::number(hostbus) +
                ",hostaddr=" + QString::number(hostaddr);
    }
    else{
        args << "-usb" << "-device" << "usb-xbox-gamepad";
    }
    args << "-display" << "sdl";
    for(auto q: args){
        std::cout << q.toStdString() << std::endl;
    }
    process.execute(sett->bin_path, args);
}

void MainWindow::on_actionRun_ISO_triggered()
{
    QString image_path = QFileDialog::getOpenFileName(this, "", "/home/dracc/Emulation/Xbox/Superdisc.iso");
    if(image_path != ""){
        RunGame(image_path);
    }
}

void MainWindow::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    RunGame(item->text());
}

void MainWindow::on_actionControls_triggered()
{
    openConfigWindow(2);
}

void MainWindow::on_actionPaths_triggered()
{
    openConfigWindow(0);
}

void MainWindow::on_actionNetwork_triggered()
{
    openConfigWindow(3);
}
