/*
   SPDX-FileCopyrightText: 2016-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "importimapsettingsakonadicheckjob.h"
#include "importimapsettingsakonadipassword.h"
#include "libsieveeditor_export.h"
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
    return filename.startsWith(QLatin1String("akonadi_kolab_resource"))
           || filename.startsWith(QLatin1String("akonadi_imap_resource"));
}

void ImportImapSettingsAkonadiCheckJob::start()
{
    mSettingsWereImported = false;
    const QStringList dirs = QStandardPaths::locateAll(QStandardPaths::ConfigLocation, QString(), QStandardPaths::LocateDirectory);
    for (const QString &dir : dirs) {
        const QStringList fileNames = QDir(dir).entryList(QStringList() << QStringLiteral("*rc"));
        for (const QString &file : fileNames) {
            if (resourceCanHaveSieveSupport(file)) {
                if (importSettings(dir, file)) {
                    mSettingsWereImported = true;
                }
            }
        }
    }
    checkNoSettingsImported();
}

bool ImportImapSettingsAkonadiCheckJob::importSettings(const QString &directory, const QString &filename)
{
    const QString filePath = directory +  QLatin1Char('/') + filename;
    //qCDebug(SIEVEEDITOR_LOG) << "importSettings filename:" << filePath;
    if (!QFileInfo::exists(filePath)) {
        qCWarning(SIEVEEDITOR_LOG) << "Unable to open file " << filePath;
        return false;
    }
    SieveEditorUtil::SieveServerConfig config;
    bool isKolabSettings = filePath.contains(QLatin1String("/akonadi_kolab_resource"));
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
        const QString encryption = networkGroup.readEntry(QStringLiteral("Safety"));
        if (encryption == QLatin1String("SSL")) {
            config.sieveImapAccountSettings.setEncryptionMode(KSieveUi::SieveImapAccountSettings::EncryptionMode::SSLorTLS);
        } else if (encryption == QLatin1String("STARTTLS")) {
            config.sieveImapAccountSettings.setEncryptionMode(KSieveUi::SieveImapAccountSettings::EncryptionMode::STARTTLS);
        } else if (encryption == QLatin1String("None")) {
            config.sieveImapAccountSettings.setEncryptionMode(KSieveUi::SieveImapAccountSettings::EncryptionMode::Unencrypted);
        } else if (encryption.isEmpty()) { //Default value
            if (isKolabSettings) {
                config.sieveImapAccountSettings.setEncryptionMode(KSieveUi::SieveImapAccountSettings::EncryptionMode::STARTTLS);
            } else {
                config.sieveImapAccountSettings.setEncryptionMode(KSieveUi::SieveImapAccountSettings::EncryptionMode::Unencrypted);
            }
        } else {
            qCWarning(SIEVEEDITOR_LOG) << "Unknown encryption mode " << encryption;
        }
        const int sievePort = sieveGroup.readEntry(QStringLiteral("SievePort"), 4190);
        if (sievePort != -1) {
            config.sieveSettings.port = sievePort;
        }
        if (reuseImapSettings) {
            config.sieveSettings.serverName = imapServerName;
            config.sieveSettings.userName = userName;
            config.sieveSettings.authenticationType
                = static_cast<MailTransport::Transport::EnumAuthenticationType::type>(sieveGroup.readEntry(QStringLiteral("Authentication"),
                                                                                                           static_cast<int>(MailTransport::Transport::EnumAuthenticationType::PLAIN)));
        } else {
            const QString sieveCustomUserName = sieveGroup.readEntry(QStringLiteral("SieveCustomUsername"));
            config.sieveSettings.userName = sieveCustomUserName;
            config.sieveSettings.serverName = imapServerName; //FIXME
            //TODO
        }
        Q_ASSERT_X(mPasswordImporter, "Missing mPasswordImporter", "You must create a mPasswordImporter");
        mPasswordImporter->importPasswords(config, filename, reuseImapSettings);
        if (config.isValid()) {
            Q_EMIT importSetting(filename, config);
            return true;
        }
    }
    return false;
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
