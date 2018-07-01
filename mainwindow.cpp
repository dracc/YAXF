#include "config.h"
#include "mainwindow.h"
#include "settings.h"
#include "settingsmanager.h"
#include "ui_mainwindow.h"

#include <iostream>
#include <QProcess>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    sett = new settings;
    initSettings(sett);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionConfigure_triggered()
{
    Config* c = new Config(this, sett);
    c->show();
}

void MainWindow::on_actionQuit_triggered()
{
    this->close();
}

void MainWindow::on_actionRun_ISO_triggered()
{
    QString image_path = QFileDialog::getOpenFileName(this, "", "/home/dracc/Emulation/Xbox/Superdisc.iso");
    if(image_path != ""){
        QProcess process;
        QStringList args;
        args << "-cpu" << "pentium3";
        args << "-machine" << "xbox,bootrom=" + sett->mcpx_path + (sett->full_boot_anim ? "":",short_animation");
        args << QString("-m") << QString(sett->expanded_ram ? "128":"64");
        args << "-bios" << sett->flash_path;
        args << "-drive" << "file=" + sett->hdd_path + ",index=0,media=disk" + (sett->hdd_unlocked ? "" : ",locked");
        args << "-drive" << "file=" + image_path + ",index=1,media=cdrom";
        //args << "-usb" << "-device" << "usb-xbox-gamepad-sdl";
        args << /*"-usb" << */"-device" << "usb-host,port=3,hostbus=001,hostaddr=014"/*",vendorid=0x45e,productid=0x202,"*/;
        args << "-display" << "sdl";
        for(auto q: args){
            std::cout << q.toStdString() << std::endl;
        }
        process.execute(sett->bin_path, args);
        //std::cout << process.errorString().toStdString() << std::endl;
    }
}
