/*
   SPDX-FileCopyrightText: 2017-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "importwizard/checks/abstractimapsettingspassword.h"

class ImportImapSettingsPasswordTest : public AbstractImapSettingsPassword
{
public:
    ImportImapSettingsPasswordTest();
    void importPasswords(const SieveEditorUtil::SieveServerConfig &config, const QString &filename, bool reuseImapSettings) override;
};
