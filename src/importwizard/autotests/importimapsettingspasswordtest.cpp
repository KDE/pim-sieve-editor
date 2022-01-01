/*
   SPDX-FileCopyrightText: 2017-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "importimapsettingspasswordtest.h"

ImportImapSettingsPasswordTest::ImportImapSettingsPasswordTest() = default;

void ImportImapSettingsPasswordTest::importPasswords(const SieveEditorUtil::SieveServerConfig &_config, const QString &filename, bool reuseImapSettings)
{
    SieveEditorUtil::SieveServerConfig config = _config;
    const QString password = QStringLiteral("password_imap");
    const QString customPassword = QStringLiteral("password_custom_sieve_imap");
    config.sieveImapAccountSettings.setPassword(password);
    if (reuseImapSettings) {
        config.sieveSettings.password = password;
    } else {
        config.sieveSettings.password = customPassword;
    }
    Q_EMIT importPasswordDone(config, filename);
}
