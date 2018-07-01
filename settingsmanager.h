#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include "settings.h"

#include <QStandardPaths>
#include <QSettings>

void initSettings(settings *sett);

void storeSetting(QString const& key, QVariant const& variant);

#endif // SETTINGSMANAGER_H
