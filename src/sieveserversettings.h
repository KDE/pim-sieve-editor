/*
   SPDX-FileCopyrightText: 2014-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>
class SieveServerSettings : public QObject
{
    Q_OBJECT
public:
    ~SieveServerSettings() override;
    static SieveServerSettings *self();

private:
    Q_DISABLE_COPY(SieveServerSettings)
    explicit SieveServerSettings(QObject *parent = nullptr);
    friend class SieveServerSettingsPrivate;
};
