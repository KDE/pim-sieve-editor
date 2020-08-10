/*
   SPDX-FileCopyrightText: 2013-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
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
        : authenticationType(MailTransport::Transport::EnumAuthenticationType::PLAIN)
        , port(-1)
    {
    }

    Q_REQUIRED_RESULT bool operator ==(const SieveAccountSettings &other) const;
    Q_REQUIRED_RESULT bool isValid() const;
    QString serverName;
    QString userName;
    QString password;
    MailTransport::Transport::EnumAuthenticationType::type authenticationType;
    int port;
};

struct LIBSIEVEEDITOR_EXPORT SieveServerConfig {
    SieveServerConfig()
        : enabled(true)
        , useImapCustomServer(false)
    {
    }

    Q_REQUIRED_RESULT bool isValid() const;
    Q_REQUIRED_RESULT QUrl url() const;
    SieveAccountSettings sieveSettings;
    KSieveUi::SieveImapAccountSettings sieveImapAccountSettings;
    bool operator ==(const SieveServerConfig &other) const;
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
LIBSIEVEEDITOR_TESTS_EXPORT QDebug operator <<(QDebug d, const SieveEditorUtil::SieveAccountSettings &settings);
LIBSIEVEEDITOR_TESTS_EXPORT QDebug operator <<(QDebug d, const SieveEditorUtil::SieveServerConfig &settings);
#endif // SIEVEEDITORUTIL_H
