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

#include "importimapsettingsakonadipassword.h"
#include "sieveeditor_debug.h"
#include "sieveserversettings.h"

using namespace QKeychain;

ImportImapSettingsAkonadiPassword::ImportImapSettingsAkonadiPassword(QObject *parent)
    : AbstractImapSettingsPassword(parent)
{
}

ImportImapSettingsAkonadiPassword::~ImportImapSettingsAkonadiPassword()
{
}

void ImportImapSettingsAkonadiPassword::readSieveServerPasswordFinished(QKeychain::Job *baseJob)
{
    auto job = qobject_cast<ReadPasswordJob *>(baseJob);
    Q_ASSERT(job);
    if (!job->error()) {
        mCurrentConfig.sieveImapAccountSettings.setPassword(job->textData());
    } else {
        qCWarning(SIEVEEDITOR_LOG) << "An error occurred while reading password: " << job->errorString();
    }

    if (mReuseImapSettings) {
        mCurrentConfig.sieveSettings.password = mCurrentConfig.sieveImapAccountSettings.password();
        Q_EMIT importPasswordDone(mCurrentConfig, mFileName);
    } else {
        loadImapAccountSettings();
    }
}

void ImportImapSettingsAkonadiPassword::readSieveServerCustomPasswordFinished(Job *baseJob)
{
    auto job = qobject_cast<ReadPasswordJob *>(baseJob);
    Q_ASSERT(job);
    if (!job->error()) {
        mCurrentConfig.sieveSettings.password = job->textData();
    } else {
        qCWarning(SIEVEEDITOR_LOG) << "An error occurred while reading password: " << job->errorString();
    }
    Q_EMIT importPasswordDone(mCurrentConfig, mFileName);
}

void ImportImapSettingsAkonadiPassword::loadImapAccountSettings()
{
    auto readJob = new ReadPasswordJob(QStringLiteral("imap"), this);
    connect(readJob, &Job::finished, this, &ImportImapSettingsAkonadiPassword::readSieveServerCustomPasswordFinished);
    readJob->setKey(QStringLiteral("custom_sieve_") + mFileName);
    readJob->start();
}

void ImportImapSettingsAkonadiPassword::importPasswords(const SieveEditorUtil::SieveServerConfig &_config, const QString &filename, bool reuseImapSettings)
{
    mCurrentConfig = _config;
    mReuseImapSettings = reuseImapSettings;
    mFileName = filename;
    auto readJob = new ReadPasswordJob(QStringLiteral("imap"), this);
    connect(readJob, &Job::finished, this, &ImportImapSettingsAkonadiPassword::readSieveServerPasswordFinished);
    readJob->setKey(mFileName);
    readJob->start();
}
