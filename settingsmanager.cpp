#include "settingsmanager.h"

void settingsManager::initSettings(settings *sett){
    // Open settings file
    QSettings settingsFile(QStandardPaths::standardLocations(QStandardPaths::AppConfigLocation).first() + "/settings.cfg", QSettings::IniFormat);

    // Reused path, let's make it simple.
    QString emulationPath = QStandardPaths::standardLocations(QStandardPaths::HomeLocation).first() + "/Emulation/Xbox";

    sett->bin_path = settingsFile.value("binPath", "./i386-softmmu/qemu-system-i386").toString();
    sett->mcpx_path = settingsFile.value("mcpxPath", emulationPath + "/mcpxv1.0.bin").toString();
    sett->flash_path = settingsFile.value("flashPath", emulationPath + "/bios.bin").toString();
    sett->xiso_path = settingsFile.value("xisoPath", emulationPath + "/").toString();
    sett->hdd_path = settingsFile.value("hddPath", emulationPath + "/xbox_hdd.qcow2").toString();
    sett->expanded_ram = settingsFile.value("expandedRAM", false).toBool();
    sett->full_boot_anim = settingsFile.value("fullBootAnim", false).toBool();
    sett->hdd_unlocked = settingsFile.value("hddUnlocked", false).toBool();
    sett->ctrl_1 = settingsFile.value("ctrl_1", 0).toInt();
    sett->ctrl_2 = settingsFile.value("ctrl_2", 0).toInt();
    sett->ctrl_3 = settingsFile.value("ctrl_3", 0).toInt();
    sett->ctrl_4 = settingsFile.value("ctrl_4", 0).toInt();
    sett->c_1_plugged = settingsFile.value("C1Plugged", false).toBool();
    sett->c_2_plugged = settingsFile.value("C2Plugged", false).toBool();
    sett->c_3_plugged = settingsFile.value("C3Plugged", false).toBool();
    sett->c_4_plugged = settingsFile.value("C4Plugged", false).toBool();
    sett->cpuaccel = settingsFile.value("enableCPUaccel", false).toBool();
}

void settingsManager::storeSetting(QString const& key, QVariant const& variant){
    // Open settings file
    QSettings settingsFile(QStandardPaths::standardLocations(QStandardPaths::AppConfigLocation).first()+ "/settings.cfg", QSettings::IniFormat);
    settingsFile.setValue(key, variant);
}

const QStringList settingsManager::genArgs(settings *sett,
                                           QString const& path,
                                           QVector<libusb_device*> const& xbox_controllers){
    QString accel = "";
    
#ifdef __linux
    accel = (sett->cpuaccel ? ",accel=kvm,kernel_irqchip=off":"");
#endif
#ifdef _WIN32
    accel = (sett->cpuaccel ? ",accel=haxm":"");
#endif
#ifdef __APPLE__
    accel = (sett->cpuaccel ? ",accel=haxm":"");
#endif
    QStringList args;
    QVector<bool> ctrlr_plugged{sett->c_1_plugged, sett->c_2_plugged, sett->c_3_plugged, sett->c_4_plugged};
    QVector<int> ctrlr_indices{sett->ctrl_1, sett->ctrl_2, sett->ctrl_3, sett->ctrl_4};
    QVector<int> ctrlr_port{3,4,1,2};
    args << "-cpu" << "pentium3";
    args << "-machine" << "xbox,bootrom=" + sett->mcpx_path +
            (sett->full_boot_anim ? "":",short_animation") +
            accel;
    args << "-m" << (sett->expanded_ram ? "128":"64");
    args << "-bios" << sett->flash_path;
    args << "-drive" << "file=" + sett->hdd_path + ",index=0,media=disk" +
            (sett->hdd_unlocked ? "" : ",locked");
    args << "-drive" << "file=" + path + ",index=1,media=cdrom";
    args << "-net" << "nic,model=nvnet" << "-net" << "user,hostfwd=tcp::9269-:9269,hostfwd=tcp::8731-:731";
    for(int i = 0; i < 4; i++){
        if(ctrlr_plugged[i]){
            if(ctrlr_indices[i] >= 2){
                int hostbus = libusb_get_bus_number(xbox_controllers.at(ctrlr_indices[i] - 2));
                int hostaddr = libusb_get_device_address(xbox_controllers.at(ctrlr_indices[i] - 2));
                args << "-usb" << "-device"
                     << "usb-host,port=" + QString::number(ctrlr_port[i]) +
                        ",hostbus=" + QString::number(hostbus) +
                        ",hostaddr=" + QString::number(hostaddr);
            }
            else{
                args << "-usb" << "-device" << "usb-xbox-gamepad,port=" +
                        QString::number(ctrlr_port[i]);
            }
        }
    }
    args << "-display" << "sdl";
    return args;
}
