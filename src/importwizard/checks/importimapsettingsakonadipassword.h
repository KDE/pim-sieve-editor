/*
   Copyright (C) 2017-2021 Laurent Montel <montel@kde.org>

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

#ifndef IMPORTIMAPSETTINGSAKONADIPASSWORD_H
#define IMPORTIMAPSETTINGSAKONADIPASSWORD_H

#include "abstractimapsettingspassword.h"
#include <qt5keychain/keychain.h>

class ImportImapSettingsAkonadiPassword : public AbstractImapSettingsPassword
{
    Q_OBJECT
public:
    explicit ImportImapSettingsAkonadiPassword(QObject *parent = nullptr);
    ~ImportImapSettingsAkonadiPassword() override;

    void importPasswords(const SieveEditorUtil::SieveServerConfig &config, const QString &filename, bool reuseImapSettings) override;

private:
    void loadImapAccountSettings();
    void readSieveServerPasswordFinished(QKeychain::Job *baseJob);
    void readSieveServerCustomPasswordFinished(QKeychain::Job *baseJob);
    SieveEditorUtil::SieveServerConfig mCurrentConfig;
    bool mReuseImapSettings = false;
};

#endif // IMPORTIMAPSETTINGSAKONADIPASSWORD_H
