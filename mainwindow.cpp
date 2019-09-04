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
    settingsManager::initSettings(sett);

    libusb_init(&context);
    num_devices = libusb_get_device_list(context, &device_list);
    for(int i = 0; i < num_devices; ++i){
        iterator = device_list[i];
        desc = {0};
        libusb_get_device_descriptor(iterator, &desc);
        if(desc.idVendor == 0x045e){
            if(desc.idProduct == 0x202 || desc.idProduct == 0x289){
                xbox_controllers.push_back(iterator);
            }
        }
    }
    QDirIterator qdit(sett->xiso_path, QStringList() << "*.iso", QDir::Files, QDirIterator::Subdirectories);
    while(qdit.hasNext()){
        QListWidgetItem *q = new QListWidgetItem(QIcon(":/dvd.svg"), qdit.next());
        ui->listWidget->addItem(q);
    }
    ui->listWidget->sortItems();
    ui->listWidget->insertItem(0, new QListWidgetItem(QIcon(":/dvd.svg"), ""));
}

MainWindow::~MainWindow()
{
    libusb_free_device_list(device_list, num_devices);
    libusb_exit(context);
    delete sett;
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
    QProcess *process = new QProcess();
    QStringList args = settingsManager::genArgs(sett, path, xbox_controllers);
    std::cout << "\n" << sett->bin_path.toStdString() << " " << std::flush;
    for(auto q: args){
        std::cout << q.toStdString() << " " << std::flush;
    }
    std::cout << std::endl;
    process->setProcessChannelMode(QProcess::ForwardedChannels);
    process->start(sett->bin_path, args);

    if(process->waitForStarted()){
        connect(process, &QProcess::started, [&](){this->hide();});
        connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), [&](){this->show();});
    }
}

void MainWindow::on_actionRun_ISO_triggered()
{
    QString image_path = QFileDialog::getOpenFileName(this, "", sett->xiso_path);
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
