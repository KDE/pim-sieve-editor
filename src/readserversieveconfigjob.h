/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "sieveeditorutil.h"
#include <QObject>
#include <qt6keychain/keychain.h>
class ReadServerSieveConfigJob : public QObject
{
    Q_OBJECT
public:
    explicit ReadServerSieveConfigJob(QObject *parent = nullptr);
    ~ReadServerSieveConfigJob() override;

    void start();
Q_SIGNALS:
    void finished(const QList<SieveEditorUtil::SieveServerConfig> &lstConfig);
    void loadNextConfig();

private:
    void loadSettings(const QString &conf);
    void slotReadNextConfig();
    void loadImapAccountSettings();
    void readSieveServerPasswordFinished(QKeychain::Job *baseJob);
    void readImapPasswordFinished(QKeychain::Job *baseJob);
    QList<SieveEditorUtil::SieveServerConfig> mLstConfig;
    SieveEditorUtil::SieveServerConfig mCurrentSieveServerConfig;
    QStringList mGroupsConfigs;
};
