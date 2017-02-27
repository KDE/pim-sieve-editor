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
#include "sieveeditor_debug.h"
#include <KLocalizedString>
#include <KConfig>
#include <KConfigGroup>
#include <QDir>
#include <QFile>
#include <QRegularExpression>

LIBSIEVEEDITOR_EXPORT QString sieveeditor_thunderbird_default_toplevel_path = QDir::homePath() + QLatin1String("/.thunderbird/");

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
    const int numberLstProfileCount = lstProfile.count();
    if (numberLstProfileCount > 0) {
        if (numberLstProfileCount == 1) {

            //Path =
            //return currentProfile;
        } else {
            QMap<QString, QString>::const_iterator i = lstProfile.constBegin();
            while (i != lstProfile.constEnd()) {
                //cout << i.key() << ": " << i.value() << endl;
                ++i;
            }
            //TODO import all settings.
        }
    }
    checkNoSettingsImported();
}

bool ImportImapSettingsThunderbirdCheckJob::importSettings(const QString &directory, const QString &filename)
{
    const QString filePath = directory +  QLatin1Char('/') + filename;
    //qCDebug(SIEVEEDITOR_LOG) << "importSettings filename:" << filePath;
    QFile file(filePath);
    if (!file.exists()) {
        qCWarning(SIEVEEDITOR_LOG) << "Unable to open file " << filePath;
        return false;
    }

    //TODO import directory
    return false;
}

QString ImportImapSettingsThunderbirdCheckJob::defaultPath() const
{
    return sieveeditor_thunderbird_default_toplevel_path;
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
