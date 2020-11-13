/*
   Copyright (C) 2014-2020 Laurent Montel <montel@kde.org>

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

void SieveServerSettings::slotWalletClosed()
{
    delete mWallet;
    mWallet = nullptr;
}
