/*
   SPDX-FileCopyrightText: 2014-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "sieveserversettings.h"

#include <KWallet>

SieveServerSettings::SieveServerSettings(QObject *parent)
    : QObject(parent)
{
}

SieveServerSettings::~SieveServerSettings()
{
    delete mWallet;
}

SieveServerSettings *SieveServerSettings::self()
{
    static SieveServerSettings s_self;
    return &s_self;
}

KWallet::Wallet *SieveServerSettings::wallet()
{
    if (!mWallet) {
        mWallet = KWallet::Wallet::openWallet(KWallet::Wallet::LocalWallet(), 0);
        if (mWallet) {
            connect(mWallet, &KWallet::Wallet::walletClosed, this, &SieveServerSettings::slotWalletClosed);
        }
    }
    return mWallet;
}

void SieveServerSettings::closeWallet()
{
    KWallet::Wallet::closeWallet(KWallet::Wallet::LocalWallet(), true);
}

void SieveServerSettings::slotWalletClosed()
{
    delete mWallet;
    mWallet = nullptr;
}
