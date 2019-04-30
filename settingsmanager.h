#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include "settings.h"

#include <QStandardPaths>
#include <QSettings>
#include <QVector>

extern "C" {
#include <libusb-1.0/libusb.h>
}

namespace settingsManager{

    void initSettings(settings *sett);

    void storeSetting(QString const& key, QVariant const& variant);

    const QStringList genArgs(settings *sett, QString const& path,
                              QVector<libusb_device*> const& xbox_controllers);

    QStandardItemModel* netRulesModelFromString(settings *sett);

    QString netRulesModelToString(settings *sett);
}
#endif // SETTINGSMANAGER_H
