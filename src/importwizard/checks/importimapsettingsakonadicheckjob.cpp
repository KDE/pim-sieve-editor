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
                importSettings(dir, file);
            }
        }
    }
}

void ImportImapSettingsAkonadiCheckJob::importSettings(const QString &directory, const QString &filename)
{
    QString filePath = directory +  QLatin1Char('/') + filename;
    qCDebug(SIEVEEDITOR_LOG) << "importSettings filename:" << filePath;
    QFile file(filePath);
    if (!file.exists()) {
        qCWarning(SIEVEEDITOR_LOG) << "Unable to open file " << filePath;
        return;
    }
    SieveEditorUtil::SieveServerConfig config;
    bool isKolabSettings = filePath.contains(QStringLiteral("/akonadi_kolab_resource"));
    KSharedConfigPtr resourceConfig = KSharedConfig::openConfig(filePath);
    KConfigGroup sieveGroup = resourceConfig->group(QStringLiteral("siever"));
    bool hasSieveSupport = sieveGroup.readEntry(QStringLiteral("SieveSupport"), isKolabSettings ? true : false);
    if (hasSieveSupport) {
        bool reuseImapSettings = sieveGroup.readEntry(QStringLiteral("SieveReuseConfig"), true);
        KConfigGroup networkGroup = resourceConfig->group(QStringLiteral("network"));
        const QString userName = networkGroup.readEntry(QStringLiteral("UserName"), QString());
        const QString imapServerName = networkGroup.readEntry(QStringLiteral("ImapServer"), QString());
        const int imapPort = networkGroup.readEntry(QStringLiteral("ImapPort"), isKolabSettings ? 143 : 993);
        config.sieveImapAccountSettings.setUserName(userName);
        config.sieveImapAccountSettings.setServerName(imapServerName);
        config.sieveImapAccountSettings.setPort(imapPort);
        config.sieveImapAccountSettings.setAuthenticationType(
                    static_cast<KSieveUi::SieveImapAccountSettings::AuthenticationMode>(
                        networkGroup.readEntry(QStringLiteral("Authentication"),
                                               static_cast<int>(KSieveUi::SieveImapAccountSettings::Plain))));
        //TODO
        //config.sieveImapAccountSettings.setEncryptionMode();
        //config.sieveImapAccountSettings.setPassword();
        const int sievePort = sieveGroup.readEntry(QStringLiteral("SievePort"), 4190);
        if (sievePort != -1) {
            config.sieveSettings.port = sievePort;
        }
        if (reuseImapSettings) {
            config.sieveSettings.serverName = imapServerName;
            config.sieveSettings.userName = userName;
            //config.sieveSettings.authenticationType =
        } else {
            const QString sieveCustomUserName = sieveGroup.readEntry(QStringLiteral("SieveCustomUsername"));
            config.sieveSettings.userName = sieveCustomUserName;
            //TODO
        }
        //TODO import kwallet settings too

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
