/*
   Copyright (C) 2017-2020 Laurent Montel <montel@kde.org>

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
#include "sieveserversettings.h"
#include <KWallet>
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
    auto *job = qobject_cast<ReadPasswordJob *>(baseJob);
    Q_ASSERT(job);
    if (!job->error()) {
//        mCurrentSieveServerConfig.sieveSettings.password = job->textData();
    } else {
//        qCWarning(SIEVEEDITOR_LOG) << "We have an error during reading password " << job->errorString();
    }

//    loadImapAccountSettings();
}

void ImportImapSettingsAkonadiPassword::importPasswords(const SieveEditorUtil::SieveServerConfig &_config, const QString &filename, bool reuseImapSettings)
{
    SieveEditorUtil::SieveServerConfig config = _config;
    mReuseImapSettings = reuseImapSettings;
    mFileName = filename;
    KWallet::Wallet *wallet = SieveServerSettings::self()->wallet();
    QString password;
    QString customPassword;
#if 0
    auto readJob = new ReadPasswordJob(QStringLiteral("imap"), this);
    connect(readJob, &Job::finished, this, &ImportImapSettingsAkonadiPassword::readSieveServerPasswordFinished);
    readJob->setKey(mFileName);
    readJob->start();
#endif

    if (wallet) {
        bool passwordStoredInWallet = false;
        //Make async
        if (wallet && wallet->hasFolder(QStringLiteral("imap"))) {
            wallet->setFolder(QStringLiteral("imap"));
            wallet->readPassword(filename, password);
            if (!reuseImapSettings) { //Custom Password
                wallet->readPassword(QStringLiteral("custom_sieve_") + filename, customPassword);
            }
            passwordStoredInWallet = true;
        }
        if (passwordStoredInWallet) {
            config.sieveImapAccountSettings.setPassword(password);
            if (reuseImapSettings) {
                config.sieveSettings.password = password;
            } else {
                config.sieveSettings.password = customPassword;
            }
        }
    }
    Q_EMIT importPasswordDone(config, mFileName);
}
