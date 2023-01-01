/*
   SPDX-FileCopyrightText: 2017-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "abstractimapsettingspassword.h"

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#include <qt5keychain/keychain.h>
#else
#include <qt6keychain/keychain.h>
#endif

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
