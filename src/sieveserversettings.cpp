/*
   SPDX-FileCopyrightText: 2014-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "sieveserversettings.h"

SieveServerSettings::SieveServerSettings(QObject *parent)
    : QObject(parent)
{
}

SieveServerSettings::~SieveServerSettings() = default;

SieveServerSettings *SieveServerSettings::self()
{
    static SieveServerSettings s_self;
    return &s_self;
}

#include "moc_sieveserversettings.cpp"
