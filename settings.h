#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>

struct settings{
public:
    QString bin_path{""};
    QString mcpx_path{""};
    QString flash_path{""};
    QString hdd_path{""};
    bool expanded_ram = false;
    bool full_boot_anim = false;
    bool hdd_unlocked = false;
    int ctrl_1 = 0;
};

#endif // SETTINGS_H
