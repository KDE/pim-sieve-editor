/*
   SPDX-FileCopyrightText: 2016-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "abstractimapsettingscheckjob.h"
#include "abstractimapsettingspassword.h"

AbstractImapSettingsCheckJob::AbstractImapSettingsCheckJob(QObject *parent)
    : QObject(parent)
{
}

AbstractImapSettingsCheckJob::~AbstractImapSettingsCheckJob()
{
    delete mPasswordImporter;
}

void AbstractImapSettingsCheckJob::setImapSettingsPassword(AbstractImapSettingsPassword *passwordImporter)
{
    mPasswordImporter = passwordImporter;
}

void AbstractImapSettingsCheckJob::checkNoSettingsImported()
{
    if (!mSettingsWereImported) {
        Q_EMIT noSettingsImported(name());
    }
    deleteLater();
}
