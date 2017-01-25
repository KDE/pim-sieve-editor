/*
   Copyright (C) 2013-2017 Montel Laurent <montel@kde.org>

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

#ifndef SIEVEEDITORUTIL_H
#define SIEVEEDITORUTIL_H

#include "sieveeditorlib_export.h"
#include <QString>
#include <QUrl>
#include <MailTransport/mailtransport/transport.h>
#include <KSieveUi/SieveImapAccountSettings>
namespace KWallet
{
class Wallet;
}
namespace SieveEditorUtil
{
struct SIEVEEDITORLIB_EXPORT SieveAccountSettings {
    SieveAccountSettings()
        : authenticationType(MailTransport::Transport::EnumAuthenticationType::PLAIN),
          port(-1)
    {

    }
    bool isValid() const;
    QString serverName;
    QString userName;
    QString password;
    MailTransport::Transport::EnumAuthenticationType::type authenticationType;
    int port;
};

struct SIEVEEDITORLIB_EXPORT SieveServerConfig {
    SieveServerConfig()
        : enabled(true),
          useImapCustomServer(false)
    {

    }

    bool isValid() const;
    QUrl url() const;
    SieveAccountSettings sieveSettings;
    KSieveUi::SieveImapAccountSettings sieveImapAccountSettings;
    bool enabled;
    bool useImapCustomServer;
};

QVector<SieveServerConfig> readServerSieveConfig();
void writeServerSieveConfig(const QVector<SieveEditorUtil::SieveServerConfig> &lstConfig);
void addServerSieveConfig(const SieveEditorUtil::SieveServerConfig &conf);
void writeSieveSettings(KWallet::Wallet *wallet, const KSharedConfigPtr &cfg, const SieveEditorUtil::SieveServerConfig &conf, int index);
void deletePasswords(const QStringList &identifiers);
QString sievePasswordIdentifier(const QString &userName, const QString &serverName);
QString imapPasswordIdentifier(const QString &userName, const QString &serverName);
}
Q_DECLARE_TYPEINFO(SieveEditorUtil::SieveServerConfig, Q_MOVABLE_TYPE);

#endif // SIEVEEDITORUTIL_H
