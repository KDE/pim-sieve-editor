/*
   SPDX-FileCopyrightText: 2017-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "abstractimapsettingspassword.h"

#include <qt6keychain/keychain.h>

class ImportImapSettingsAkonadiPassword : public AbstractImapSettingsPassword
{
    Q_OBJECT
public:
    explicit ImportImapSettingsAkonadiPassword(QObject *parent = nullptr);
    ~ImportImapSettingsAkonadiPassword() override;

    void importPasswords(const SieveEditorUtil::SieveServerConfig &config, const QString &filename, bool reuseImapSettings) override;

private:
    void loadImapAccountSettings();
    void readSieveServerPasswordFinished(QKeychain::Job *baseJob);
    void readSieveServerCustomPasswordFinished(QKeychain::Job *baseJob);
    SieveEditorUtil::SieveServerConfig mCurrentConfig;
    bool mReuseImapSettings = false;
};
