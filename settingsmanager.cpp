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
    sett->eeprom_path = settingsFile.value("eepromPath", "(Optional)").toString();
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
    sett->sdl_gl = settingsFile.value("enableSDLGL", false).toBool();
    sett->netRules = settingsFile.value("netRules", "").toString();
    sett->netRulesModel = netRulesModelFromString(sett);
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
            (sett->full_boot_anim ? "" : ",short_animation") +
            accel + ((sett->eeprom_path == "(Optional)") ? "" : ",eeprom=" + sett->eeprom_path);
    args << "-m" << (sett->expanded_ram ? "128" : "64");
    args << "-bios" << sett->flash_path;
    args << "-drive" << "file=" + sett->hdd_path + ",index=0,media=disk" +
            (sett->hdd_unlocked ? "" : ",locked");
    args << "-drive" << "file=" + path + ",index=1,media=cdrom";
    if (sett->netRulesModel->rowCount() > 0) {
        args << "-net" << "nic,model=nvnet" << "-net";
        QString netRules = "user";
        for (int i = 0; i < sett->netRulesModel->rowCount(); ++i) {
            QString protocol = sett->netRulesModel->item(i, 0)->text();
            QString guestport = sett->netRulesModel->item(i, 1)->text();
            QString hostport = sett->netRulesModel->item(i, 2)->text();
            netRules += ",hostfwd=" + protocol + "::" + hostport + "-:" + guestport;
        }
        args << netRules;
    }
    for(int i = 0; i < 4; i++){
        if(ctrlr_plugged[i]){
            if(ctrlr_indices[i] >= 2){
                int hostbus = libusb_get_bus_number(xbox_controllers.at(ctrlr_indices[i] - 2));
                int hostaddr = libusb_get_device_address(xbox_controllers.at(ctrlr_indices[i] - 2));
                args << "-usb"
                     << "-device" << "usb-hub,port=" +
                        QString::number(ctrlr_port[i])
                     << "-device"
                     << "usb-host,port=" + QString::number(ctrlr_port[i]) + ".1" +
                        ",hostbus=" + QString::number(hostbus) +
                        ",hostaddr=" + QString::number(hostaddr);
            }
            else{
                args << "-device" << "usb-hub,port=" +
                        QString::number(ctrlr_port[i]) <<
                        "-device" << "usb-xbox-gamepad,port=" +
                        QString::number(ctrlr_port[i]) + ".1";
            }
        }
    }
    args << "-display" << QString("sdl") + QString(sett->sdl_gl ? ",gl=on": "");
    return args;
}

QStandardItemModel *settingsManager::netRulesModelFromString(settings *sett)
{
    auto ret = new QStandardItemModel(0, 3);
    ret->setHorizontalHeaderLabels({"Protocol", "Guest port", "Host port"});
    if (sett->netRules == "") {
        return ret;
    }
    QStringList splitValues = sett->netRules.split(" ");
    int numRows = splitValues[0].toInt();
    splitValues.pop_front();
    for (int i = 0; i < numRows; ++i) {
        ret->insertRow(i);
        int q = i * 3;
        ret->setItem(i, 0, new QStandardItem(splitValues[q]));
        ret->setItem(i, 1, new QStandardItem(splitValues[q+1]));
        ret->setItem(i, 2, new QStandardItem(splitValues[q+2]));
    }
    return ret;
}

QString settingsManager::netRulesModelToString(settings *sett)
{
    QStringList ret;
    int rows = sett->netRulesModel->rowCount();
    ret << QString::number(rows);
    for (int i = 0; i < rows; ++i) {
        ret << sett->netRulesModel->item(i, 0)->text();
        ret << sett->netRulesModel->item(i, 1)->text();
        ret << sett->netRulesModel->item(i, 2)->text();
    }
    return ret.join(" ");
}
