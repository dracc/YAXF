#include "settingsmanager.h"

void initSettings(settings *sett){
    // Open settings file
    QSettings settingsFile(QStandardPaths::standardLocations(QStandardPaths::AppConfigLocation).first() + "/settings.cfg", QSettings::IniFormat);

    // Reused path, let's make it simple.
    QString emulationPath = QStandardPaths::standardLocations(QStandardPaths::HomeLocation).first() + "/Emulation/Xbox";

    sett->bin_path = settingsFile.value("binPath", "./i386-softmmu/qemu-system-i386").toString();
    sett->mcpx_path = settingsFile.value("mcpxPath", emulationPath + "/mcpxv1.0.bin").toString();
    sett->flash_path = settingsFile.value("flashPath", emulationPath + "/bios.bin").toString();
    sett->hdd_path = settingsFile.value("hddPath", emulationPath + "/xbox_hdd.qcow2").toString();
    sett->expanded_ram = settingsFile.value("expandedRAM", false).toBool();
    sett->full_boot_anim = settingsFile.value("fullBootAnim", false).toBool();
    sett->hdd_unlocked = settingsFile.value("hddUnlocked", false).toBool();
}

void storeSetting(QString const& key, QVariant const& variant){
    // Open settings file
    QSettings settingsFile(QStandardPaths::standardLocations(QStandardPaths::AppConfigLocation).first()+ "/settings.cfg", QSettings::IniFormat);
    settingsFile.setValue(key, variant);
}
