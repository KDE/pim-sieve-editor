/*
   Copyright (C) 2013-2020 Laurent Montel <montel@kde.org>

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

#include "sieveeditorutil.h"
#include "sieveserversettings.h"
#include "sieveeditor_debug.h"
#include "sieveeditorsavepasswordjob.h"

#include <KConfig>

#include <KConfigGroup>

#include <QRegularExpression>
#include <QUrlQuery>
#include <KSharedConfig>

#include <qt5keychain/keychain.h>
using namespace QKeychain;


QUrl SieveEditorUtil::SieveServerConfig::url() const
{
    QUrl u;
    u.setHost(sieveSettings.serverName);
    u.setUserName(sieveSettings.userName);
    u.setPassword(sieveSettings.password);
    u.setPort(sieveSettings.port);

    QString authStr;
    switch (sieveSettings.authenticationType) {
    case MailTransport::Transport::EnumAuthenticationType::CLEAR:
    case MailTransport::Transport::EnumAuthenticationType::PLAIN:
        authStr = QStringLiteral("PLAIN");
        break;
    case MailTransport::Transport::EnumAuthenticationType::LOGIN:
        authStr = QStringLiteral("LOGIN");
        break;
    case MailTransport::Transport::EnumAuthenticationType::CRAM_MD5:
        authStr = QStringLiteral("CRAM-MD5");
        break;
    case MailTransport::Transport::EnumAuthenticationType::DIGEST_MD5:
        authStr = QStringLiteral("DIGEST-MD5");
        break;
    case MailTransport::Transport::EnumAuthenticationType::GSSAPI:
        authStr = QStringLiteral("GSSAPI");
        break;
    case MailTransport::Transport::EnumAuthenticationType::ANONYMOUS:
        authStr = QStringLiteral("ANONYMOUS");
        break;
    default:
        authStr = QStringLiteral("PLAIN");
        break;
    }
    QUrlQuery query;
    query.addQueryItem(QStringLiteral("x-mech"), authStr);
    u.setQuery(query);
    return u;
}

bool SieveEditorUtil::SieveServerConfig::operator ==(const SieveEditorUtil::SieveServerConfig &other) const
{
    const bool result = (enabled == other.enabled)
                        && (useImapCustomServer == other.useImapCustomServer)
                        && (sieveSettings == other.sieveSettings)
                        && (sieveImapAccountSettings == other.sieveImapAccountSettings);
    if (!result) {
        qCDebug(SIEVEEDITOR_LOG) << "enabled " << enabled << " other.enabled " << other.enabled;
        qCDebug(SIEVEEDITOR_LOG) << "useImapCustomServer " << useImapCustomServer << " other.useImapCustomServer " << other.useImapCustomServer;
        qCDebug(SIEVEEDITOR_LOG) << "sieveSettings " << sieveSettings << " other.sieveSettings " << other.sieveSettings;
        qCDebug(SIEVEEDITOR_LOG) << "sieveImapAccountSettings " << sieveImapAccountSettings << " other.sieveImapAccountSettings " << other.sieveImapAccountSettings;
    }
    return result;
}


void SieveEditorUtil::writeServerSieveConfig(const QVector<SieveServerConfig> &lstConfig)
{
    KSharedConfigPtr cfg = KSharedConfig::openConfig();
    const QRegularExpression re(QStringLiteral("^ServerSieve (.+)$"));
    //Delete Old Group
    const QStringList groups = cfg->groupList().filter(re);
    for (const QString &conf : groups) {
        KConfigGroup group = cfg->group(conf);
        group.deleteGroup();
    }

    int i = 0;
    for (const SieveEditorUtil::SieveServerConfig &conf : lstConfig) {
        writeSieveSettings(cfg, conf, i);
        ++i;
    }
    cfg->sync();
    cfg->reparseConfiguration();
}

QString SieveEditorUtil::sievePasswordIdentifier(const QString &userName, const QString &serverName)
{
    return userName + QLatin1Char('@') + serverName;
}

QString SieveEditorUtil::imapPasswordIdentifier(const QString &userName, const QString &serverName)
{
    return QLatin1String("Imap") + userName + QLatin1Char('@') + serverName;
}

void SieveEditorUtil::writeSieveSettings(const KSharedConfigPtr &cfg, const SieveEditorUtil::SieveServerConfig &conf, int index)
{
    KConfigGroup group = cfg->group(QStringLiteral("ServerSieve %1").arg(index));
    group.writeEntry(QStringLiteral("Port"), conf.sieveSettings.port);
    group.writeEntry(QStringLiteral("ServerName"), conf.sieveSettings.serverName);
    group.writeEntry(QStringLiteral("UserName"), conf.sieveSettings.userName);
    group.writeEntry(QStringLiteral("Enabled"), conf.enabled);

    const QString walletEntry = SieveEditorUtil::sievePasswordIdentifier(conf.sieveSettings.userName, conf.sieveSettings.serverName);
    auto *writeJob = new SieveEditorSavePasswordJob;
    writeJob->setName(SieveEditorUtil::walletFolderName());
    writeJob->setPassword(conf.sieveSettings.password);
    writeJob->setKey(walletEntry);

    writeJob->start();

    group.writeEntry(QStringLiteral("Authentication"), static_cast<int>(conf.sieveSettings.authenticationType));

    //Imap Account Settings
    group.writeEntry(QStringLiteral("ImapPort"), conf.sieveImapAccountSettings.port());
    group.writeEntry(QStringLiteral("ImapAuthentication"), static_cast<int>(conf.sieveImapAccountSettings.authenticationType()));
    group.writeEntry(QStringLiteral("ImapEncrypt"), static_cast<int>(conf.sieveImapAccountSettings.encryptionMode()));

    if ((conf.sieveImapAccountSettings.serverName() != conf.sieveSettings.serverName) && (conf.sieveImapAccountSettings.userName() != conf.sieveSettings.userName)
        && !conf.sieveImapAccountSettings.serverName().isEmpty()
        && !conf.sieveImapAccountSettings.userName().isEmpty()) {
        group.writeEntry(QStringLiteral("useImapCustomServer"), true);
        group.writeEntry(QStringLiteral("ImapServerName"), conf.sieveImapAccountSettings.serverName());
        group.writeEntry(QStringLiteral("ImapUserName"), conf.sieveImapAccountSettings.userName());
        const QString imapWalletEntry = imapPasswordIdentifier(conf.sieveImapAccountSettings.userName(), conf.sieveImapAccountSettings.serverName());

        auto *writeImapSettingJob = new SieveEditorSavePasswordJob;
        writeImapSettingJob->setName(SieveEditorUtil::walletFolderName());
        writeImapSettingJob->setPassword(conf.sieveImapAccountSettings.password());
        writeImapSettingJob->setKey(imapWalletEntry);
        writeImapSettingJob->start();
    }
}

QString SieveEditorUtil::walletFolderName()
{
    return QStringLiteral("sieveeditor");
}

void SieveEditorUtil::addServerSieveConfig(const SieveEditorUtil::SieveServerConfig &conf)
{
    KSharedConfigPtr cfg = KSharedConfig::openConfig();
    const QRegularExpression re(QStringLiteral("^ServerSieve (.+)$"));
    const QStringList groups = cfg->groupList().filter(re);

    writeSieveSettings(cfg, conf, groups.count());
    cfg->sync();
}

QDebug operator <<(QDebug d, const SieveEditorUtil::SieveServerConfig &settings)
{
    d << "sieveSettings " << settings.sieveSettings;
    d << "sieveImapAccountSettings " << settings.sieveImapAccountSettings;
    d << "url " << settings.url();
    d << "enabled " << settings.enabled;
    d << "useImapCustomServer " << settings.useImapCustomServer;
    return d;
}

QDebug operator <<(QDebug d, const SieveEditorUtil::SieveAccountSettings &settings)
{
    d << "serverName " << settings.serverName;
    d << "userName " << settings.userName;
    d << "password " << settings.password;
    d << "authenticationType " << settings.authenticationType;
    d << "port " << settings.port;
    return d;
}

bool SieveEditorUtil::SieveAccountSettings::operator ==(const SieveEditorUtil::SieveAccountSettings &other) const
{
    bool result = (serverName == other.serverName)
                  && (userName == other.userName)
                  && (password == other.password)
                  && (authenticationType == other.authenticationType)
                  && (port == other.port);
    if (!result) {
        qCDebug(SIEVEEDITOR_LOG) << "serverName " << serverName << " other.serverName " << other.serverName;
        qCDebug(SIEVEEDITOR_LOG) << "userName " << userName << " other.userName " << other.userName;
        qCDebug(SIEVEEDITOR_LOG) << "password " << password << " other.password " << other.password;
        qCDebug(SIEVEEDITOR_LOG) << "authenticationType " << authenticationType << " other.authenticationType " << other.authenticationType;
        qCDebug(SIEVEEDITOR_LOG) << "port " << port << " other.port " << other.port;
    }
    return result;
}

bool SieveEditorUtil::SieveAccountSettings::isValid() const
{
    return !serverName.isEmpty() && !userName.isEmpty() && (port != -1);
}

bool SieveEditorUtil::SieveServerConfig::isValid() const
{
    return sieveSettings.isValid();
}
