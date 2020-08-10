/*
   SPDX-FileCopyrightText: 2017-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef IMPORTIMAPSETTINGSPASSWORDTEST_H
#define IMPORTIMAPSETTINGSPASSWORDTEST_H

#include <importwizard/checks/abstractimapsettingspassword.h>

class ImportImapSettingsPasswordTest : public AbstractImapSettingsPassword
{
public:
    ImportImapSettingsPasswordTest();
    void importPasswords(SieveEditorUtil::SieveServerConfig &config, const QString &filename, bool reuseImapSettings) override;
};

#endif // IMPORTIMAPSETTINGSPASSWORDTEST_H
