/*
   Copyright (C) 2017 Montel Laurent <montel@kde.org>

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

#include "importimapsettingspasswordtest.h"

ImportImapSettingsPasswordTest::ImportImapSettingsPasswordTest()
{

}

void ImportImapSettingsPasswordTest::importPasswords(SieveEditorUtil::SieveServerConfig &config, const KSharedConfigPtr &resourceConfig, bool reuseImapSettings)
{
    Q_UNUSED(resourceConfig);
    const QString password = QStringLiteral("password_imap");
    const QString customPassword = QStringLiteral("password_custom_sieve_imap");
    config.sieveImapAccountSettings.setPassword(password);
    if (reuseImapSettings) {
        config.sieveSettings.password = password;
    } else {
        config.sieveSettings.password = customPassword;
    }
}
