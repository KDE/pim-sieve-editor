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
    qDebug() << " thunderbirdPath"<<thunderbirdPath;
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
    qCDebug(SIEVEEDITOR_LOG) << "importSettings filename:" << filePath;
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qCWarning(SIEVEEDITOR_LOG) << "Unable to open file " << filePath;
        return false;
    }
    QTextStream stream(&file);
    while (!stream.atEnd()) {
        const QString line = stream.readLine();
        if (line.startsWith(QStringLiteral("user_pref"))) {
            if (line.contains(QStringLiteral("mail.server.")) ||
                    line.contains(QStringLiteral("mail.account.")) ||
                    line.contains(QStringLiteral("mail.accountmanager."))||
                    line.contains(QStringLiteral("extensions.sieve.account."))) {
                insertIntoMap(line);
            }

        } else {
            if (!line.startsWith(QLatin1Char('#')) &&
                    line.isEmpty() &&
                    line.startsWith(QStringLiteral("/*")) &&
                    line.startsWith(QStringLiteral(" */")) &&
                    line.startsWith(QStringLiteral(" *"))) {
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
            qCDebug(SIEVEEDITOR_LOG) << "imap account " << accountName;
            const QString host = mHashConfig.value(accountName + QStringLiteral(".hostname")).toString();
            const QString userName = mHashConfig.value(accountName + QStringLiteral(".userName")).toString();
            const QString name = mHashConfig.value(accountName + QStringLiteral(".name")).toString();
            //TODO
            atLeastAnAccountFound = true;
        } else {
            qCDebug(SIEVEEDITOR_LOG) << "Account " << accountName << " is not a imap account. Skip it.";
        }

    }
    return atLeastAnAccountFound;
}

//Stolen from import-wizard

void ImportImapSettingsThunderbirdCheckJob::addAuth(QMap<QString, QVariant> &settings, const QString &argument, const QString &accountName)
{
    bool found = false;
    if (mHashConfig.contains(accountName + QStringLiteral(".authMethod"))) {
        const int authMethod = mHashConfig.value(accountName + QStringLiteral(".authMethod")).toInt(&found);
        if (found) {
            switch (authMethod) {
            case 0:
                break;
            case 4: //Encrypted password ???
                settings.insert(argument, MailTransport::Transport::EnumAuthenticationType::LOGIN);   //????
                qCDebug(SIEVEEDITOR_LOG) << " authmethod == encrypt password";
                break;
            case 5: //GSSAPI
                settings.insert(argument, MailTransport::Transport::EnumAuthenticationType::GSSAPI);
                break;
            case 6: //NTLM
                settings.insert(argument, MailTransport::Transport::EnumAuthenticationType::NTLM);
                break;
            case 7: //TLS
                qCDebug(SIEVEEDITOR_LOG) << " authmethod method == TLS"; //????
                break;
            default:
                qCDebug(SIEVEEDITOR_LOG) << " ThunderbirdSettings::addAuth unknown :" << authMethod;
                break;
            }
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
