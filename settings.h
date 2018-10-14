#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>

struct settings{
public:
    QString bin_path{""};
    QString mcpx_path{""};
    QString flash_path{""};
    QString hdd_path{""};
    QString xiso_path{""};
    bool expanded_ram = false;
    bool full_boot_anim = false;
    bool hdd_unlocked = false;
    int ctrl_1 = 0;
    int ctrl_2 = 0;
    int ctrl_3 = 0;
    int ctrl_4 = 0;
    bool c_1_plugged = false;
    bool c_2_plugged = false;
    bool c_3_plugged = false;
    bool c_4_plugged = false;
    bool cpuaccel = false;
};

#endif // SETTINGS_H
