/*
   SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "libsieveeditor_export.h"
#include "libsieveeditor_private_export.h"
#include <KSieveUi/SieveImapAccountSettings>
#include <MailTransport/Transport>
#include <QString>
#include <QUrl>
namespace SieveEditorUtil
{
struct LIBSIEVEEDITOR_EXPORT SieveAccountSettings {
    SieveAccountSettings() = default;

    Q_REQUIRED_RESULT bool operator==(const SieveAccountSettings &other) const;
    Q_REQUIRED_RESULT bool isValid() const;
    QString serverName;
    QString userName;
    QString password;
    MailTransport::Transport::EnumAuthenticationType::type authenticationType = MailTransport::Transport::EnumAuthenticationType::PLAIN;
    int port = -1;
};

struct LIBSIEVEEDITOR_EXPORT SieveServerConfig {
    SieveServerConfig() = default;

    Q_REQUIRED_RESULT bool isValid() const;
    Q_REQUIRED_RESULT QUrl url() const;
    SieveAccountSettings sieveSettings;
    KSieveUi::SieveImapAccountSettings sieveImapAccountSettings;
    bool operator==(const SieveServerConfig &other) const;
    bool enabled = true;
    bool useImapCustomServer = false;
};

void writeServerSieveConfig(const QVector<SieveEditorUtil::SieveServerConfig> &lstConfig);
void addServerSieveConfig(const SieveEditorUtil::SieveServerConfig &conf);
void writeSieveSettings(const KSharedConfigPtr &cfg, const SieveEditorUtil::SieveServerConfig &conf, int index);
Q_REQUIRED_RESULT QString sievePasswordIdentifier(const QString &userName, const QString &serverName);
Q_REQUIRED_RESULT QString imapPasswordIdentifier(const QString &userName, const QString &serverName);
Q_REQUIRED_RESULT QString walletFolderName();
}
Q_DECLARE_TYPEINFO(SieveEditorUtil::SieveServerConfig, Q_MOVABLE_TYPE);
LIBSIEVEEDITOR_TESTS_EXPORT QDebug operator<<(QDebug d, const SieveEditorUtil::SieveAccountSettings &settings);
LIBSIEVEEDITOR_TESTS_EXPORT QDebug operator<<(QDebug d, const SieveEditorUtil::SieveServerConfig &settings);
