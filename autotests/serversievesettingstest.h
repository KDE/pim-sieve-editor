/*
   SPDX-FileCopyrightText: 2014-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef SERVERSIEVESETTINGSTEST_H
#define SERVERSIEVESETTINGSTEST_H

#include <QObject>

class ServerSieveSettingsTest : public QObject
{
    Q_OBJECT
public:
    explicit ServerSieveSettingsTest(QObject *parent = nullptr);
    ~ServerSieveSettingsTest();

private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldSetValue();
    void shouldEmitEnableOkButtonSignal();
    void shouldEmitSignalWithValueTrue();
};

#endif // SERVERSIEVESETTINGSTEST_H
