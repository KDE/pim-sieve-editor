/*
   SPDX-FileCopyrightText: 2017-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "importimapsettingsakonadipassword.h"
#include "sieveeditor_debug.h"
#include "sieveserversettings.h"

using namespace QKeychain;

ImportImapSettingsAkonadiPassword::ImportImapSettingsAkonadiPassword(QObject *parent)
    : AbstractImapSettingsPassword(parent)
{
}

ImportImapSettingsAkonadiPassword::~ImportImapSettingsAkonadiPassword() = default;

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

#include "moc_importimapsettingsakonadipassword.cpp"
