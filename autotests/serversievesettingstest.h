/*
   SPDX-FileCopyrightText: 2014-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class ServerSieveSettingsTest : public QObject
{
    Q_OBJECT
public:
    explicit ServerSieveSettingsTest(QObject *parent = nullptr);
    ~ServerSieveSettingsTest() override;

private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldSetValue();
    void shouldEmitEnableOkButtonSignal();
    void shouldEmitSignalWithValueTrue();
};
