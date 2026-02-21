/*
   SPDX-FileCopyrightText: 2017-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "abstractimapsettingspassword.h"

AbstractImapSettingsPassword::AbstractImapSettingsPassword(QObject *parent)
    : QObject(parent)
{
}

AbstractImapSettingsPassword::~AbstractImapSettingsPassword() = default;

void AbstractImapSettingsPassword::importPasswords([[maybe_unused]] const SieveEditorUtil::SieveServerConfig &config,
                                                   [[maybe_unused]] const QString &filename,
                                                   [[maybe_unused]] bool reuseImapSettings)
{
}

#include "moc_abstractimapsettingspassword.cpp"
