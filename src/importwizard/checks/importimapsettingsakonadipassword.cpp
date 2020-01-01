/*
   Copyright (C) 2017-2020 Laurent Montel <montel@kde.org>

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
