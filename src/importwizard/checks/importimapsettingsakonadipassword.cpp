/*
   SPDX-FileCopyrightText: 2017-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "importimapsettingsakonadipassword.h"
#include "sieveserversettings.h"
#include <KWallet>

ImportImapSettingsAkonadiPassword::ImportImapSettingsAkonadiPassword()
    : AbstractImapSettingsPassword()
{
}

ImportImapSettingsAkonadiPassword::~ImportImapSettingsAkonadiPassword()
{
}

void ImportImapSettingsAkonadiPassword::importPasswords(SieveEditorUtil::SieveServerConfig &config, const QString &filename, bool reuseImapSettings)
{
    KWallet::Wallet *wallet = SieveServerSettings::self()->wallet();
    QString password;
    QString customPassword;
    if (wallet) {
        bool passwordStoredInWallet = false;
        if (wallet && wallet->hasFolder(QStringLiteral("imap"))) {
            wallet->setFolder(QStringLiteral("imap"));
            wallet->readPassword(filename, password);
            if (!reuseImapSettings) { //Custom Password
                wallet->readPassword(QStringLiteral("custom_sieve_") + filename, customPassword);
            }
            passwordStoredInWallet = true;
        }
        if (passwordStoredInWallet) {
            config.sieveImapAccountSettings.setPassword(password);
            if (reuseImapSettings) {
                config.sieveSettings.password = password;
            } else {
                config.sieveSettings.password = customPassword;
            }
        }
    }
}
