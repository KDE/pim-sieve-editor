/*
   Copyright (C) 2013-2016 Montel Laurent <montel@kde.org>

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

#include <kwallet.h>

#include <KConfig>

#include <KConfigGroup>

#include <QRegularExpression>
#include <QUrlQuery>
#include <KSharedConfig>

QUrl SieveEditorUtil::SieveServerConfig::url() const
{
    QUrl u;
    u.setHost(account.serverName());
    u.setUserName(account.userName());
    u.setPassword(account.password());
    u.setPort(account.port());

    QString authStr;
    switch (account.authenticationType()) {
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

QVector<SieveEditorUtil::SieveServerConfig> SieveEditorUtil::readServerSieveConfig()
{
    QVector<SieveServerConfig> lstConfig;
    KSharedConfigPtr cfg = KSharedConfig::openConfig();
    QRegularExpression re(QStringLiteral("^ServerSieve (.+)$"));
    const QStringList groups = cfg->groupList().filter(re);
    KWallet::Wallet *wallet = SieveServerSettings::self()->wallet();
    if (wallet && !wallet->setFolder(QStringLiteral("sieveeditor"))) {
        wallet->createFolder(QStringLiteral("sieveeditor"));
        wallet->setFolder(QStringLiteral("sieveeditor"));
    }

    Q_FOREACH (const QString &conf, groups) {
        SieveServerConfig sieve;
        KConfigGroup group = cfg->group(conf);
        sieve.account.setPort(group.readEntry(QStringLiteral("Port"), 0));
        sieve.account.setServerName(group.readEntry(QStringLiteral("ServerName")));
        sieve.account.setUserName(group.readEntry(QStringLiteral("UserName")));
        sieve.enabled = group.readEntry(QStringLiteral("Enabled"), true);
        const QString walletEntry = sieve.account.userName() + QLatin1Char('@') + sieve.account.serverName();
        if (wallet && wallet->hasEntry(walletEntry)) {
            QString passwd;
            wallet->readPassword(walletEntry, passwd);
            sieve.account.setPassword(passwd);
        }
        sieve.account.setAuthenticationType(static_cast<MailTransport::Transport::EnumAuthenticationType::type>(group.readEntry(QStringLiteral("Authentication"), static_cast<int>(MailTransport::Transport::EnumAuthenticationType::PLAIN))));
        lstConfig.append(sieve);
    }
    return lstConfig;
}

void SieveEditorUtil::writeServerSieveConfig(const QVector<SieveServerConfig> &lstConfig)
{
    KSharedConfigPtr cfg = KSharedConfig::openConfig();
    const QRegularExpression re(QStringLiteral("^ServerSieve (.+)$"));
    //Delete Old Group
    const QStringList groups = cfg->groupList().filter(re);
    Q_FOREACH (const QString &conf, groups) {
        KConfigGroup group = cfg->group(conf);
        group.deleteGroup();
    }

    int i = 0;
    KWallet::Wallet *wallet = SieveServerSettings::self()->wallet();
    if (wallet) {
        if (!wallet->hasFolder(QStringLiteral("sieveeditor"))) {
            wallet->createFolder(QStringLiteral("sieveeditor"));
        }
        wallet->setFolder(QStringLiteral("sieveeditor"));
    }

    Q_FOREACH (const SieveEditorUtil::SieveServerConfig &conf, lstConfig) {
        writeSieveSettings(wallet, cfg, conf, i);
        ++i;
    }
    cfg->sync();
    cfg->reparseConfiguration();
}

void SieveEditorUtil::writeSieveSettings(KWallet::Wallet *wallet, KSharedConfigPtr cfg, const SieveEditorUtil::SieveServerConfig &conf, int index)
{
    KConfigGroup group = cfg->group(QStringLiteral("ServerSieve %1").arg(index));
    group.writeEntry(QStringLiteral("Port"), conf.account.port());
    group.writeEntry(QStringLiteral("ServerName"), conf.account.serverName());
    group.writeEntry(QStringLiteral("UserName"), conf.account.userName());
    group.writeEntry(QStringLiteral("Enabled"), conf.enabled);
    const QString walletEntry = conf.account.userName() + QLatin1Char('@') + conf.account.serverName();
    if (wallet) {
        wallet->writePassword(walletEntry, conf.account.password());
    }
    group.writeEntry(QStringLiteral("Authentication"), static_cast<int>(conf.account.authenticationType()));
}

void SieveEditorUtil::addServerSieveConfig(const SieveEditorUtil::SieveServerConfig &conf)
{
    KWallet::Wallet *wallet = SieveServerSettings::self()->wallet();
    if (wallet) {
        if (!wallet->hasFolder(QStringLiteral("sieveeditor"))) {
            wallet->createFolder(QStringLiteral("sieveeditor"));
        }
        wallet->setFolder(QStringLiteral("sieveeditor"));
    }
    KSharedConfigPtr cfg = KSharedConfig::openConfig();
    const QRegularExpression re(QStringLiteral("^ServerSieve (.+)$"));
    const QStringList groups = cfg->groupList().filter(re);

    writeSieveSettings(wallet, cfg, conf, groups.count());
    cfg->sync();
}
