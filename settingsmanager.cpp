#include "settingsmanager.h"

void initSettings(settings *sett){
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
}

void storeSetting(QString const& key, QVariant const& variant){
    // Open settings file
    QSettings settingsFile(QStandardPaths::standardLocations(QStandardPaths::AppConfigLocation).first()+ "/settings.cfg", QSettings::IniFormat);
    settingsFile.setValue(key, variant);
}
