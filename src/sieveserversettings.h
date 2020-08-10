/*
   SPDX-FileCopyrightText: 2014-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef SieveServerSettings_H
#define SieveServerSettings_H

#include <QObject>
namespace KWallet {
class Wallet;
}
class SieveServerSettings : public QObject
{
    Q_OBJECT
public:
    ~SieveServerSettings();
    static SieveServerSettings *self();

    KWallet::Wallet *wallet();

    void closeWallet();
private:
    void slotWalletClosed();

private:
    Q_DISABLE_COPY(SieveServerSettings)
    explicit SieveServerSettings(QObject *parent = nullptr);
    friend class SieveServerSettingsPrivate;
    KWallet::Wallet *mWallet = nullptr;
};

#endif // SieveServerSettings_H
