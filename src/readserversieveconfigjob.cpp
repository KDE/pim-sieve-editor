/*
   Copyright (C) 2020 Laurent Montel <montel@kde.org>

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

#include "readserversieveconfigjob.h"

#include "sieveserversettings.h"
#include "sieveeditor_debug.h"

#include <KSharedConfig>
#include <KConfig>
#include <QRegularExpression>

using namespace QKeychain;

ReadServerSieveConfigJob::ReadServerSieveConfigJob(QObject *parent)
    : QObject(parent)
{
    connect(this, &ReadServerSieveConfigJob::loadNextConfig, this, &ReadServerSieveConfigJob::slotReadNextConfig);
}

ReadServerSieveConfigJob::~ReadServerSieveConfigJob()
{
}

void ReadServerSieveConfigJob::loadSettings(const QString &conf)
{
    mCurrentSieveServerConfig = {};
    KSharedConfigPtr cfg = KSharedConfig::openConfig();
    KConfigGroup group = cfg->group(conf);
    //Sieve Account Settings
    mCurrentSieveServerConfig.sieveSettings.port = group.readEntry(QStringLiteral("Port"), 0);
    mCurrentSieveServerConfig.sieveSettings.serverName = group.readEntry(QStringLiteral("ServerName"));
    mCurrentSieveServerConfig.sieveSettings.userName = group.readEntry(QStringLiteral("UserName"));
    mCurrentSieveServerConfig.enabled = group.readEntry(QStringLiteral("Enabled"), true);
    mCurrentSieveServerConfig.sieveSettings.authenticationType
        = static_cast<MailTransport::Transport::EnumAuthenticationType::type>(group.readEntry(QStringLiteral("Authentication"),
                                                                                              static_cast<int>(MailTransport::Transport::EnumAuthenticationType::PLAIN)));

    //Imap Account Settings
    mCurrentSieveServerConfig.sieveImapAccountSettings.setPort(group.readEntry(QStringLiteral("ImapPort"), 0));
    mCurrentSieveServerConfig.sieveImapAccountSettings.setServerName(group.readEntry(QStringLiteral("ImapServerName")));
    mCurrentSieveServerConfig.sieveImapAccountSettings.setUserName(group.readEntry(QStringLiteral("ImapUserName")));
    mCurrentSieveServerConfig.sieveImapAccountSettings.setAuthenticationType(
        static_cast<KSieveUi::SieveImapAccountSettings::AuthenticationMode>(group.readEntry(QStringLiteral("ImapAuthentication"), static_cast<int>(KSieveUi::SieveImapAccountSettings::Plain))));
    mCurrentSieveServerConfig.sieveImapAccountSettings.setEncryptionMode(
        static_cast<KSieveUi::SieveImapAccountSettings::EncryptionMode>(group.readEntry(QStringLiteral("ImapEncrypt"), static_cast<int>(KSieveUi::SieveImapAccountSettings::SSLorTLS))));

    const QString walletEntry = mCurrentSieveServerConfig.sieveSettings.userName + QLatin1Char('@') + mCurrentSieveServerConfig.sieveSettings.serverName;
    auto readJob = new ReadPasswordJob(SieveEditorUtil::walletFolderName(), this);
    connect(readJob, &Job::finished, this, &ReadServerSieveConfigJob::readSieveServerPasswordFinished);
    readJob->setKey(walletEntry);
    readJob->start();
}

void ReadServerSieveConfigJob::readSieveServerPasswordFinished(QKeychain::Job *baseJob)
{
    auto job = qobject_cast<ReadPasswordJob *>(baseJob);
    Q_ASSERT(job);
    if (!job->error()) {
        mCurrentSieveServerConfig.sieveSettings.password = job->textData();
    } else {
        qCWarning(SIEVEEDITOR_LOG) << "We have an error during reading password " << job->errorString();
    }

    loadImapAccountSettings();
}

void ReadServerSieveConfigJob::loadImapAccountSettings()
{
    if (!mCurrentSieveServerConfig.sieveImapAccountSettings.userName().isEmpty()
        && !mCurrentSieveServerConfig.sieveImapAccountSettings.serverName().isEmpty()
        && (mCurrentSieveServerConfig.sieveImapAccountSettings.userName() != mCurrentSieveServerConfig.sieveSettings.userName)
        && (mCurrentSieveServerConfig.sieveImapAccountSettings.serverName() != mCurrentSieveServerConfig.sieveSettings.serverName)) {
        mCurrentSieveServerConfig.useImapCustomServer = true;

        const QString imapWalletEntry = QLatin1String("Imap") + mCurrentSieveServerConfig.sieveImapAccountSettings.userName() + QLatin1Char('@') + mCurrentSieveServerConfig.sieveImapAccountSettings.serverName();
        auto readJob = new ReadPasswordJob(SieveEditorUtil::walletFolderName(), this);
        connect(readJob, &Job::finished, this, &ReadServerSieveConfigJob::readImapPasswordFinished);
        readJob->setKey(imapWalletEntry);
        readJob->start();
    } else {
        //Use Sieve Account Settings
        mCurrentSieveServerConfig.sieveImapAccountSettings.setUserName(mCurrentSieveServerConfig.sieveSettings.userName);
        mCurrentSieveServerConfig.sieveImapAccountSettings.setServerName(mCurrentSieveServerConfig.sieveSettings.serverName);
        mCurrentSieveServerConfig.sieveImapAccountSettings.setPassword(mCurrentSieveServerConfig.sieveSettings.password);
        mCurrentSieveServerConfig.useImapCustomServer = false;
        mLstConfig.append(mCurrentSieveServerConfig);
        Q_EMIT loadNextConfig();
    }
}

void ReadServerSieveConfigJob::readImapPasswordFinished(QKeychain::Job *baseJob)
{
    auto job = qobject_cast<ReadPasswordJob *>(baseJob);
    Q_ASSERT(job);
    if (!job->error()) {
        mCurrentSieveServerConfig.sieveImapAccountSettings.setPassword(job->textData());
    } else {
        qCWarning(SIEVEEDITOR_LOG) << "We have an error during reading password (imap) " << job->errorString();
    }
    mLstConfig.append(mCurrentSieveServerConfig);
    Q_EMIT loadNextConfig();
}

void ReadServerSieveConfigJob::start()
{
    KSharedConfigPtr cfg = KSharedConfig::openConfig();
    QRegularExpression re(QStringLiteral("^ServerSieve (.+)$"));
    mGroupsConfigs = cfg->groupList().filter(re);

    slotReadNextConfig();
}

void ReadServerSieveConfigJob::slotReadNextConfig()
{
    if (mGroupsConfigs.isEmpty()) {
        Q_EMIT finished(mLstConfig);
        deleteLater();
    } else {
        const QString conf = mGroupsConfigs.takeFirst();
        loadSettings(conf);
    }
}
