/*
   SPDX-FileCopyrightText: 2017-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "abstractimapsettingspassword.h"

AbstractImapSettingsPassword::AbstractImapSettingsPassword()
{
}

AbstractImapSettingsPassword::~AbstractImapSettingsPassword()
{
}

void AbstractImapSettingsPassword::importPasswords(SieveEditorUtil::SieveServerConfig &config, const QString &filename, bool reuseImapSettings)
{
    Q_UNUSED(config);
    Q_UNUSED(filename);
    Q_UNUSED(reuseImapSettings);
}
