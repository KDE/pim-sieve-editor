/*
   SPDX-FileCopyrightText: 2014-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "serversievesettingstest.h"
#include "../src/serversievesettings.h"
#include <QSignalSpy>
#include <QTest>
ServerSieveSettingsTest::ServerSieveSettingsTest(QObject *parent)
    : QObject(parent)
{
}

ServerSieveSettingsTest::~ServerSieveSettingsTest() = default;

void ServerSieveSettingsTest::shouldHaveDefaultValue()
{
    ServerSieveSettings widget;
    QVERIFY(widget.serverName().isEmpty());
    QVERIFY(widget.userName().isEmpty());
    QVERIFY(widget.password().isEmpty());
}

void ServerSieveSettingsTest::shouldSetValue()
{
    const QString password = QStringLiteral("password");
    const QString username = QStringLiteral("username");
    const QString servername = QStringLiteral("servername");
    ServerSieveSettings widget;
    widget.setServerName(servername);
    widget.setUserName(username);
    widget.setPassword(password);
    QCOMPARE(widget.serverName(), servername);
    QCOMPARE(widget.userName(), username);
    QCOMPARE(widget.password(), password);
}

void ServerSieveSettingsTest::shouldEmitEnableOkButtonSignal()
{
    ServerSieveSettings widget;
    widget.show();
    QVERIFY(QTest::qWaitForWindowExposed(&widget));
    QSignalSpy spy(&widget, &ServerSieveSettings::enableOkButton);
    widget.setPassword(QStringLiteral("foo"));
    QCOMPARE(spy.count(), 0);

    int numberEmitSignal = 1;
    widget.setServerName(QStringLiteral("foo"));
    QCOMPARE(spy.count(), numberEmitSignal);
    ++numberEmitSignal;
    widget.setUserName(QStringLiteral("foo"));
    QCOMPARE(spy.count(), numberEmitSignal);
    ++numberEmitSignal;
    widget.setUserName(QString());
    QCOMPARE(spy.count(), numberEmitSignal);
    ++numberEmitSignal;
    widget.setServerName(QString());
    QCOMPARE(spy.count(), numberEmitSignal);
}

void ServerSieveSettingsTest::shouldEmitSignalWithValueTrue()
{
    ServerSieveSettings widget;
    widget.show();
    QVERIFY(QTest::qWaitForWindowExposed(&widget));
    QSignalSpy spy(&widget, &ServerSieveSettings::enableOkButton);
    widget.setServerName(QStringLiteral("foo"));
    QCOMPARE(spy.count(), 1);
    // We need servername!=empty and username != empty
    QCOMPARE(spy.at(0).at(0).toBool(), false);

    widget.setUserName(QStringLiteral("foo"));
    QCOMPARE(spy.count(), 2);
    QCOMPARE(spy.at(1).at(0).toBool(), true);

    // We don't want empty string
    widget.setUserName(QStringLiteral(" "));
    QCOMPARE(spy.count(), 3);
    QCOMPARE(spy.at(2).at(0).toBool(), false);

    // We don't want empty string
    widget.setServerName(QStringLiteral(" "));
    QCOMPARE(spy.count(), 4);
    QCOMPARE(spy.at(3).at(0).toBool(), false);
}

QTEST_MAIN(ServerSieveSettingsTest)
