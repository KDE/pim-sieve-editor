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

#include "importimapsettingsakonadicheckjob.h"
#include "sieveeditor_debug.h"
#include <KLocalizedString>
#include <KConfigGroup>
#include <KSharedConfig>
#include <QStandardPaths>
#include <QDir>
#include <QFile>

ImportImapSettingsAkonadiCheckJob::ImportImapSettingsAkonadiCheckJob(QObject *parent)
    : AbstractImapSettingsCheckJob(parent)
{

}

ImportImapSettingsAkonadiCheckJob::~ImportImapSettingsAkonadiCheckJob()
{

}

bool ImportImapSettingsAkonadiCheckJob::resourceCanHaveSieveSupport(const QString &filename) const
{
    return filename.startsWith(QStringLiteral("akonadi_kolab_resource")) ||
                        filename.startsWith(QStringLiteral("akonadi_imap_resource"));
}

void ImportImapSettingsAkonadiCheckJob::start()
{
    const QStringList dirs = QStandardPaths::locateAll(QStandardPaths::ConfigLocation, QString(), QStandardPaths::LocateDirectory);
    for (const QString &dir : dirs) {
        const QStringList fileNames = QDir(dir).entryList(QStringList() << QStringLiteral("*rc"));
        for (const QString &file : fileNames) {
            if (resourceCanHaveSieveSupport(file)) {
                importSettings(dir + QLatin1Char('/') + file);
            }
        }
    }
}

void ImportImapSettingsAkonadiCheckJob::importSettings(const QString &filename)
{
    qCDebug(SIEVEEDITOR_LOG) << "importSettings filename:" << filename;
    QFile file(filename);
    if (!file.exists()) {
        qCWarning(SIEVEEDITOR_LOG) << "Unable to open file " << filename;
        return;
    }
    SieveEditorUtil::SieveServerConfig config;
    bool isKolabSettings = filename.startsWith(QStringLiteral("akonadi_kolab_resource"));
    KSharedConfigPtr resourceConfig = KSharedConfig::openConfig(filename);
    KConfigGroup sieveGroup = resourceConfig->group(QStringLiteral("siever"));
    bool hasSieveSupport = sieveGroup.readEntry(QStringLiteral("SieveSupport"), isKolabSettings ? true : false);
    if (hasSieveSupport) {

        bool reuseImapSettings = sieveGroup.readEntry(QStringLiteral("SieveReuseConfig"), true);

        KConfigGroup networkGroup = resourceConfig->group(QStringLiteral("network"));
        const QString userName = networkGroup.readEntry(QStringLiteral("UserName"), QString());
        const QString imapServerName = networkGroup.readEntry(QStringLiteral("ImapServer"), QString());
        config.sieveImapAccountSettings.setUserName(userName);
        config.sieveImapAccountSettings.setServerName(imapServerName);
        if (reuseImapSettings) {
            config.sieveSettings.serverName = imapServerName;
            config.sieveSettings.userName = userName;
        }
        //TODO save other settings
        if (config.isValid()) {
            //TODO fix name!
            Q_EMIT importSetting(filename, config);
        }
    }
}

bool ImportImapSettingsAkonadiCheckJob::settingsCanBeImported() const
{
    const QStringList dirs = QStandardPaths::locateAll(QStandardPaths::ConfigLocation, QString(), QStandardPaths::LocateDirectory);
    for (const QString &dir : dirs) {
        const QStringList fileNames = QDir(dir).entryList(QStringList() << QStringLiteral("*rc"));
        for (const QString &file : fileNames) {
            if (resourceCanHaveSieveSupport(file)) {
                return true;
            }
        }
    }

    return false;
}

QString ImportImapSettingsAkonadiCheckJob::name() const
{
    return i18n("Akonadi IMAP Settings");
}
