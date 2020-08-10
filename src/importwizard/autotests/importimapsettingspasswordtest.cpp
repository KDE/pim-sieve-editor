/*
   SPDX-FileCopyrightText: 2017-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "importimapsettingspasswordtest.h"

ImportImapSettingsPasswordTest::ImportImapSettingsPasswordTest()
{
}

void ImportImapSettingsPasswordTest::importPasswords(SieveEditorUtil::SieveServerConfig &config, const QString &filename, bool reuseImapSettings)
{
    Q_UNUSED(filename);
    const QString password = QStringLiteral("password_imap");
    const QString customPassword = QStringLiteral("password_custom_sieve_imap");
    config.sieveImapAccountSettings.setPassword(password);
    if (reuseImapSettings) {
        config.sieveSettings.password = password;
    } else {
        config.sieveSettings.password = customPassword;
    }
}
