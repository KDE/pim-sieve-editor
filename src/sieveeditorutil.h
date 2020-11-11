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

#ifndef SIEVEEDITORUTIL_H
#define SIEVEEDITORUTIL_H

#include "libsieveeditor_export.h"
#include "libsieveeditor_private_export.h"
#include <QString>
#include <QUrl>
#include <MailTransport/Transport>
#include <KSieveUi/SieveImapAccountSettings>
namespace KWallet {
class Wallet;
}
namespace SieveEditorUtil {
struct LIBSIEVEEDITOR_EXPORT SieveAccountSettings {
    SieveAccountSettings()
    {
    }

    Q_REQUIRED_RESULT bool operator ==(const SieveAccountSettings &other) const;
    Q_REQUIRED_RESULT bool isValid() const;
    QString serverName;
    QString userName;
    QString password;
    MailTransport::Transport::EnumAuthenticationType::type authenticationType = MailTransport::Transport::EnumAuthenticationType::PLAIN;
    int port = -1;
};

struct LIBSIEVEEDITOR_EXPORT SieveServerConfig {
    SieveServerConfig()
    {
    }

    Q_REQUIRED_RESULT bool isValid() const;
    Q_REQUIRED_RESULT QUrl url() const;
    SieveAccountSettings sieveSettings;
    KSieveUi::SieveImapAccountSettings sieveImapAccountSettings;
    bool operator ==(const SieveServerConfig &other) const;
    bool enabled = true;
    bool useImapCustomServer = false;
};

void writeServerSieveConfig(const QVector<SieveEditorUtil::SieveServerConfig> &lstConfig);
void addServerSieveConfig(const SieveEditorUtil::SieveServerConfig &conf);
void writeSieveSettings(KWallet::Wallet *wallet, const KSharedConfigPtr &cfg, const SieveEditorUtil::SieveServerConfig &conf, int index);
Q_REQUIRED_RESULT QString sievePasswordIdentifier(const QString &userName, const QString &serverName);
Q_REQUIRED_RESULT QString imapPasswordIdentifier(const QString &userName, const QString &serverName);
Q_REQUIRED_RESULT KWallet::Wallet *selectWalletFolder();
}
Q_DECLARE_TYPEINFO(SieveEditorUtil::SieveServerConfig, Q_MOVABLE_TYPE);
LIBSIEVEEDITOR_TESTS_EXPORT QDebug operator <<(QDebug d, const SieveEditorUtil::SieveAccountSettings &settings);
LIBSIEVEEDITOR_TESTS_EXPORT QDebug operator <<(QDebug d, const SieveEditorUtil::SieveServerConfig &settings);
#endif // SIEVEEDITORUTIL_H
