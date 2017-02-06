/*
   Copyright (C) 2016-2017 Montel Laurent <montel@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "importimapsettingsthunderbirdcheckjob.h"
#include <KLocalizedString>
#include <KConfig>
#include <KConfigGroup>
#include <QDir>
#include <QFile>
#include <QRegularExpression>

ImportImapSettingsThunderbirdCheckJob::ImportImapSettingsThunderbirdCheckJob(QObject *parent)
    : AbstractImapSettingsCheckJob(parent)
{

}

ImportImapSettingsThunderbirdCheckJob::~ImportImapSettingsThunderbirdCheckJob()
{

}

//Copy from mailimporter
QMap<QString, QString> ImportImapSettingsThunderbirdCheckJob::listProfile(QString &currentProfile, const QString &defaultSettingPath)
{
    const QString thunderbirdPath = defaultSettingPath + QLatin1String("/profiles.ini");
    QMap<QString, QString> lstProfile;
    QFile profiles(thunderbirdPath);
    if (profiles.exists()) {
        //ini file.
        KConfig config(thunderbirdPath);
        const QStringList profileList = config.groupList().filter(QRegularExpression(QStringLiteral("Profile\\d+")));
        const bool uniqProfile = (profileList.count() == 1);
        if (uniqProfile) {
            KConfigGroup group = config.group(profileList.at(0));
            const QString path = group.readEntry("Path");
            const QString name = group.readEntry(QStringLiteral("Name"));
            currentProfile = path;
            lstProfile.insert(name, path);
            return lstProfile;
        } else {
            for (const QString &profileName : profileList) {
                KConfigGroup group = config.group(profileName);
                const QString path = group.readEntry("Path");
                const QString name = group.readEntry(QStringLiteral("Name"));
                if (group.hasKey("Default") && (group.readEntry("Default", 0) == 1)) {
                    currentProfile = path;
                }
                lstProfile.insert(name, path);
            }
        }
    }
    return lstProfile;
}


void ImportImapSettingsThunderbirdCheckJob::start()
{
    mSettingsWereImported = false;
    QString currentProfile;
    const QMap<QString, QString> lstProfile = listProfile(currentProfile, defaultPath());
    if (lstProfile.isEmpty()) {
        //return QString();
    } else if (lstProfile.count() == 1) {
        //return currentProfile;
    } else {
    }
    checkNoSettingsImported();
}

bool ImportImapSettingsThunderbirdCheckJob::importSettings(const QString &directory, const QString &filename)
{
    return false;
}

QString ImportImapSettingsThunderbirdCheckJob::defaultPath() const
{
    return QDir::homePath() + QLatin1String("/.thunderbird/");
}

bool ImportImapSettingsThunderbirdCheckJob::settingsCanBeImported() const
{
    const QDir dir(defaultPath());
    return dir.exists();
}

QString ImportImapSettingsThunderbirdCheckJob::name() const
{
    return i18n("Thunderbird IMAP Settings");
}
