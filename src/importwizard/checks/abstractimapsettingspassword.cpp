/*
   SPDX-FileCopyrightText: 2017-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "abstractimapsettingspassword.h"

AbstractImapSettingsPassword::AbstractImapSettingsPassword(QObject *parent)
    : QObject(parent)
{
}

AbstractImapSettingsPassword::~AbstractImapSettingsPassword() = default;

void AbstractImapSettingsPassword::importPasswords(const SieveEditorUtil::SieveServerConfig &config, const QString &filename, bool reuseImapSettings)
{
    Q_UNUSED(config)
    Q_UNUSED(filename)
    Q_UNUSED(reuseImapSettings)
}
