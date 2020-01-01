/*
   Copyright (C) 2016-2020 Laurent Montel <montel@kde.org>

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
    if (QFileInfo::exists(thunderbirdPath)) {
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
            mSettingsWereImported = importSettings(defaultPath(), lstProfile.first());
        } else {
            QMap<QString, QString>::const_iterator i = lstProfile.constBegin();
            while (i != lstProfile.constEnd()) {
                const bool imported = importSettings(defaultPath(), i.value());
                if (imported) {
                    mSettingsWereImported = true;
                }
                ++i;
            }
        }
    }
    checkNoSettingsImported();
}

bool ImportImapSettingsThunderbirdCheckJob::importSettings(const QString &directory, const QString &defaultProfile)
{
    const QString filePath = directory +  QLatin1Char('/') + defaultProfile + QStringLiteral("/prefs.js");
    //qCDebug(SIEVEEDITOR_LOG) << "importSettings filename:" << filePath;
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qCWarning(SIEVEEDITOR_LOG) << "Unable to open file " << filePath;
        return false;
    }
    QTextStream stream(&file);
    while (!stream.atEnd()) {
        const QString line = stream.readLine();
        if (line.startsWith(QLatin1String("user_pref"))) {
            if (line.contains(QLatin1String("mail.server."))
                || line.contains(QLatin1String("mail.account."))
                || line.contains(QLatin1String("mail.accountmanager."))
                || line.contains(QLatin1String("extensions.sieve.account."))) {
                insertIntoMap(line);
            }
        } else {
            if (!line.startsWith(QLatin1Char('#'))
                && line.isEmpty()
                && line.startsWith(QLatin1String("/*"))
                && line.startsWith(QLatin1String(" */"))
                && line.startsWith(QLatin1String(" *"))) {
                qCDebug(SIEVEEDITOR_LOG) << " unstored line :" << line;
            }
        }
    }
    const QString mailAccountPreference = mHashConfig.value(QStringLiteral("mail.accountmanager.accounts")).toString();
    if (mailAccountPreference.isEmpty()) {
        qCDebug(SIEVEEDITOR_LOG) << "No account found";
        return false;
    }
    const QStringList accountList = mailAccountPreference.split(QLatin1Char(','));

    bool atLeastAnAccountFound = false;
    for (const QString &account : accountList) {
        const QString serverName = mHashConfig.value(QStringLiteral("mail.account.%1").arg(account) + QStringLiteral(".server")).toString();
        const QString accountName = QStringLiteral("mail.server.%1").arg(serverName);
        const QString type = mHashConfig.value(accountName + QStringLiteral(".type")).toString();
        if (type == QLatin1String("imap")) {
            //Sieve settings == username@account

            const QString imapServerName = mHashConfig.value(accountName + QStringLiteral(".hostname")).toString();
            QString userName = mHashConfig.value(accountName + QStringLiteral(".userName")).toString();
            //Convert @ in username in %40
            QString userNameSieveConverted = userName;
            userNameSieveConverted.replace(QLatin1Char('@'), QStringLiteral("%40"));
            const QString sieveKeyServerUserName = QLatin1String("extensions.sieve.account.") + userNameSieveConverted + QLatin1Char('@') + imapServerName;
            //user_pref("extensions.sieve.account.<username>@<server>.enabled", true);
            if (mHashConfig.value(sieveKeyServerUserName + QStringLiteral(".enabled"), false).toBool()) {
                //TODO
                //user_pref("extensions.sieve.account.<username>@<server>.TLS", true);
                //user_pref("extensions.sieve.account.<username>@<server>.TLS.forced", true);
                //user_pref("extensions.sieve.account.<username>@<server>.activeAuthorization", 1);
                //user_pref("extensions.sieve.account.<username>@<server>.activeHost", 1);
                //user_pref("extensions.sieve.account.<username>@<server>.activeLogin", 1);
                SieveEditorUtil::SieveServerConfig config;
                //0 4190
                //1 2000
                //2 custom
                //Default == 4190
                //user_pref("extensions.sieve.account.<username>@<server>.port", 1255);
                config.sieveSettings.port = mHashConfig.value(sieveKeyServerUserName + QStringLiteral(".port"), 4190).toInt();
                //not necessary to import this one : user_pref("extensions.sieve.account.<username>@<server>.port.type", 1);

                //user_pref("extensions.sieve.account.<username>@<server>.hostname", "sdfsfsqsdf");
                const QString sieveHostName = mHashConfig.value(sieveKeyServerUserName + QStringLiteral(".hostname")).toString();
                if (sieveHostName.isEmpty()) {
                    config.sieveSettings.serverName = imapServerName;
                } else {
                    config.sieveSettings.serverName = sieveHostName;
                }

                const QString sieveUserName = mHashConfig.value(sieveKeyServerUserName + QStringLiteral(".login.username")).toString();
                //user_pref("extensions.sieve.account.<username>@<server>.login.username", "newuser");
                if (sieveUserName.isEmpty()) {
                    config.sieveSettings.userName = userName;
                } else {
                    config.sieveSettings.userName = sieveUserName;
                }

                //not necessary to import this one : user_pref("extensions.sieve.account.<username>@<server>.proxy.type", 1);

                //qCDebug(SIEVEEDITOR_LOG) << "imap account " << accountName;
                const QString name = mHashConfig.value(accountName + QStringLiteral(".name")).toString();
                bool found;
                const int sievePort = mHashConfig.value(accountName + QStringLiteral(".port")).toInt(&found);
                if (found) {
                    config.sieveImapAccountSettings.setPort(sievePort);
                }
                encryption(config, accountName);
                addAuth(config, accountName);
                config.sieveImapAccountSettings.setUserName(userName);
                config.sieveImapAccountSettings.setServerName(imapServerName);

                if (config.isValid()) {
                    atLeastAnAccountFound = true;
                    Q_EMIT importSetting(name, config);
                }
            }
        } else {
            //qCDebug(SIEVEEDITOR_LOG) << "Account " << accountName << " is not a imap account. Skip it.";
        }
    }
    return atLeastAnAccountFound;
}

//Stolen from import-wizard
void ImportImapSettingsThunderbirdCheckJob::encryption(SieveEditorUtil::SieveServerConfig &config, const QString &accountName)
{
    bool found;
    const int socketType = mHashConfig.value(accountName + QStringLiteral(".socketType")).toInt(&found);
    if (found) {
        switch (socketType) {
        case 0:
            //None
            config.sieveImapAccountSettings.setEncryptionMode(KSieveUi::SieveImapAccountSettings::EncryptionMode::Unencrypted);
            break;
        case 2:
            //STARTTLS
            config.sieveImapAccountSettings.setEncryptionMode(KSieveUi::SieveImapAccountSettings::EncryptionMode::TlsV1);
            break;
        case 3:
            //SSL/TLS
            config.sieveImapAccountSettings.setEncryptionMode(KSieveUi::SieveImapAccountSettings::EncryptionMode::SslV3_1);
            break;
        default:
            qCDebug(SIEVEEDITOR_LOG) << "Unknown encryption mode " << socketType;
        }
    } else {
        qCDebug(SIEVEEDITOR_LOG) << "Invalid encryption mode settings";
    }
}

void ImportImapSettingsThunderbirdCheckJob::addAuth(SieveEditorUtil::SieveServerConfig &config, const QString &accountName)
{
    bool found = false;
    if (mHashConfig.contains(accountName + QStringLiteral(".authMethod"))) {
        const int authMethod = mHashConfig.value(accountName + QStringLiteral(".authMethod")).toInt(&found);
        if (found) {
            switch (authMethod) {
            case 0:
                config.sieveImapAccountSettings.setAuthenticationType(KSieveUi::SieveImapAccountSettings::Plain);
                break;
            case 4: //Encrypted password ???
                config.sieveImapAccountSettings.setAuthenticationType(KSieveUi::SieveImapAccountSettings::Login);
                qCDebug(SIEVEEDITOR_LOG) << " authmethod == encrypt password";
                break;
            case 5: //GSSAPI
                config.sieveImapAccountSettings.setAuthenticationType(KSieveUi::SieveImapAccountSettings::GSSAPI);
                break;
            case 6: //NTLM
                config.sieveImapAccountSettings.setAuthenticationType(KSieveUi::SieveImapAccountSettings::NTLM);
                break;
            case 7: //TLS
                qCDebug(SIEVEEDITOR_LOG) << " authmethod method == TLS"; //????
                break;
            case 10: //OAuth2
                config.sieveImapAccountSettings.setAuthenticationType(KSieveUi::SieveImapAccountSettings::XOAuth2);
                qCDebug(SIEVEEDITOR_LOG) << " authmethod method == OAuth2"; //????
                break;
            default:
                qCDebug(SIEVEEDITOR_LOG) << " ImportImapSettingsThunderbirdCheckJob::addAuth unknown :" << authMethod;
                break;
            }
        } else {
            qCDebug(SIEVEEDITOR_LOG) << " ImportImapSettingsThunderbirdCheckJob::addAuth undefine value";
        }
    }
}

void ImportImapSettingsThunderbirdCheckJob::insertIntoMap(const QString &line)
{
    QString newLine = line;
    newLine.remove(QStringLiteral("user_pref(\""));
    newLine.remove(QStringLiteral(");"));
    const int pos = newLine.indexOf(QLatin1Char(','));
    QString key = newLine.left(pos);
    key.remove(key.length() - 1, 1);
    QString valueStr = newLine.right(newLine.length() - pos - 2);
    if (valueStr.at(0) == QLatin1Char('"')) {
        valueStr.remove(0, 1);
        const int pos(valueStr.length() - 1);
        if (valueStr.at(pos) == QLatin1Char('"')) {
            valueStr.remove(pos, 1);
        }
        //Store as String
        mHashConfig.insert(key, valueStr);
    } else {
        if (valueStr == QLatin1String("true")) {
            mHashConfig.insert(key, true);
        } else if (valueStr == QLatin1String("false")) {
            mHashConfig.insert(key, false);
        } else {
            //Store as integer
            const int value = valueStr.toInt();
            mHashConfig.insert(key, value);
        }
    }
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
