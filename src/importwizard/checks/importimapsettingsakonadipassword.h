/*
   SPDX-FileCopyrightText: 2017-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef IMPORTIMAPSETTINGSAKONADIPASSWORD_H
#define IMPORTIMAPSETTINGSAKONADIPASSWORD_H

#include "abstractimapsettingspassword.h"

class ImportImapSettingsAkonadiPassword : public AbstractImapSettingsPassword
{
public:
    ImportImapSettingsAkonadiPassword();
    ~ImportImapSettingsAkonadiPassword() override;

    void importPasswords(SieveEditorUtil::SieveServerConfig &config, const QString &filename, bool reuseImapSettings) override;
};

#endif // IMPORTIMAPSETTINGSAKONADIPASSWORD_H
