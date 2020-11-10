/*
   Copyright (C) 2020 Laurent Montel <montel@kde.org>

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

#include "readserversieveconfigjob.h"

#include "sieveserversettings.h"

#include <KSharedConfig>
#include <KWallet>
#include <KConfig>
#include <QRegularExpression>
#include <QVector>

ReadServerSieveConfigJob::ReadServerSieveConfigJob(QObject *parent)
    : QObject(parent)
{

}

ReadServerSieveConfigJob::~ReadServerSieveConfigJob()
{

}

void ReadServerSieveConfigJob::start()
{
    QVector<SieveEditorUtil::SieveServerConfig> lstConfig;
    KSharedConfigPtr cfg = KSharedConfig::openConfig();
    QRegularExpression re(QStringLiteral("^ServerSieve (.+)$"));
    const QStringList groups = cfg->groupList().filter(re);
    KWallet::Wallet *wallet = SieveServerSettings::self()->wallet();
    if (wallet && !wallet->setFolder(QStringLiteral("sieveeditor"))) {
        wallet->createFolder(QStringLiteral("sieveeditor"));
        wallet->setFolder(QStringLiteral("sieveeditor"));
    }

    for (const QString &conf : groups) {
        SieveEditorUtil::SieveServerConfig sieve;
        KConfigGroup group = cfg->group(conf);
        //Sieve Account Settings
        sieve.sieveSettings.port = group.readEntry(QStringLiteral("Port"), 0);
        sieve.sieveSettings.serverName = group.readEntry(QStringLiteral("ServerName"));
        sieve.sieveSettings.userName = group.readEntry(QStringLiteral("UserName"));
        sieve.enabled = group.readEntry(QStringLiteral("Enabled"), true);
        const QString walletEntry = sieve.sieveSettings.userName + QLatin1Char('@') + sieve.sieveSettings.serverName;
        if (wallet && wallet->hasEntry(walletEntry)) {
            QString passwd;
            wallet->readPassword(walletEntry, passwd);
            sieve.sieveSettings.password = passwd;
        }
        sieve.sieveSettings.authenticationType
                = static_cast<MailTransport::Transport::EnumAuthenticationType::type>(group.readEntry(QStringLiteral("Authentication"),
                                                                                                      static_cast<int>(MailTransport::Transport::EnumAuthenticationType::PLAIN)));

        //Imap Account Settings
        sieve.sieveImapAccountSettings.setPort(group.readEntry(QStringLiteral("ImapPort"), 0));
        sieve.sieveImapAccountSettings.setServerName(group.readEntry(QStringLiteral("ImapServerName")));
        sieve.sieveImapAccountSettings.setUserName(group.readEntry(QStringLiteral("ImapUserName")));
        sieve.sieveImapAccountSettings.setAuthenticationType(
                    static_cast<KSieveUi::SieveImapAccountSettings::AuthenticationMode>(group.readEntry(QStringLiteral("ImapAuthentication"), static_cast<int>(KSieveUi::SieveImapAccountSettings::Plain))));
        sieve.sieveImapAccountSettings.setEncryptionMode(
                    static_cast<KSieveUi::SieveImapAccountSettings::EncryptionMode>(group.readEntry(QStringLiteral("ImapEncrypt"), static_cast<int>(KSieveUi::SieveImapAccountSettings::SSLorTLS))));

        if (!sieve.sieveImapAccountSettings.userName().isEmpty()
                && !sieve.sieveImapAccountSettings.serverName().isEmpty()
                && (sieve.sieveImapAccountSettings.userName() != sieve.sieveSettings.userName)
                && (sieve.sieveImapAccountSettings.serverName() != sieve.sieveSettings.serverName)) {
            const QString imapWalletEntry = QLatin1String("Imap") + sieve.sieveImapAccountSettings.userName() + QLatin1Char('@') + sieve.sieveImapAccountSettings.serverName();
            if (wallet && wallet->hasEntry(imapWalletEntry)) {
                QString passwd;
                wallet->readPassword(imapWalletEntry, passwd);
                sieve.sieveImapAccountSettings.setPassword(passwd);
            }
            sieve.useImapCustomServer = true;
        } else {
            //Use Sieve Account Settings
            sieve.sieveImapAccountSettings.setUserName(sieve.sieveSettings.userName);
            sieve.sieveImapAccountSettings.setServerName(sieve.sieveSettings.serverName);
            sieve.sieveImapAccountSettings.setPassword(sieve.sieveSettings.password);
            sieve.useImapCustomServer = false;
        }
        lstConfig.append(sieve);
    }
    //return lstConfig;
}
