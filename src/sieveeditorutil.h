/*
   SPDX-FileCopyrightText: 2013-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "libsieveeditor_export.h"
#include "libsieveeditor_private_export.h"
#include <KSieveCore/SieveImapAccountSettings>
#include <MailTransport/Transport>
#include <QString>
#include <QUrl>
namespace SieveEditorUtil
{
struct LIBSIEVEEDITOR_EXPORT SieveAccountSettings {
    SieveAccountSettings() = default;

    [[nodiscard]] bool operator==(const SieveAccountSettings &other) const;
    [[nodiscard]] bool isValid() const;
    QString serverName;
    QString userName;
    QString password;
    MailTransport::Transport::EnumAuthenticationType authenticationType = MailTransport::Transport::EnumAuthenticationType::PLAIN;
    int port = -1;
};

struct LIBSIEVEEDITOR_EXPORT SieveServerConfig {
    SieveServerConfig() = default;

    [[nodiscard]] bool isValid() const;
    [[nodiscard]] QUrl url() const;
    SieveAccountSettings sieveSettings;
    KSieveCore::SieveImapAccountSettings sieveImapAccountSettings;
    bool operator==(const SieveServerConfig &other) const;
    bool enabled = true;
    bool useImapCustomServer = false;
};

void writeServerSieveConfig(const QList<SieveEditorUtil::SieveServerConfig> &lstConfig);
void addServerSieveConfig(const SieveEditorUtil::SieveServerConfig &conf);
void writeSieveSettings(const KSharedConfigPtr &cfg, const SieveEditorUtil::SieveServerConfig &conf, int index);
[[nodiscard]] QString sievePasswordIdentifier(const QString &userName, const QString &serverName);
[[nodiscard]] QString imapPasswordIdentifier(const QString &userName, const QString &serverName);
[[nodiscard]] QString walletFolderName();
}
Q_DECLARE_TYPEINFO(SieveEditorUtil::SieveServerConfig, Q_RELOCATABLE_TYPE);
LIBSIEVEEDITOR_TESTS_EXPORT QDebug operator<<(QDebug d, const SieveEditorUtil::SieveAccountSettings &settings);
LIBSIEVEEDITOR_TESTS_EXPORT QDebug operator<<(QDebug d, const SieveEditorUtil::SieveServerConfig &settings);
