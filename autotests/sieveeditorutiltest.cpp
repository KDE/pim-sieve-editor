/*
   SPDX-FileCopyrightText: 2014-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "sieveeditorutiltest.h"
#include "../src/sieveeditorutil.h"
#include <QTest>

SieveEditorUtilTest::SieveEditorUtilTest(QObject *parent)
    : QObject(parent)
{
}

SieveEditorUtilTest::~SieveEditorUtilTest() = default;

void SieveEditorUtilTest::shouldHaveDefaultValue()
{
    SieveEditorUtil::SieveServerConfig config;
    QVERIFY(config.sieveSettings.userName.isEmpty());
    QVERIFY(config.sieveSettings.password.isEmpty());
    QVERIFY(config.sieveSettings.serverName.isEmpty());
    QCOMPARE(config.sieveSettings.authenticationType, MailTransport::Transport::EnumAuthenticationType::PLAIN);
    QCOMPARE(config.sieveSettings.port, -1);
    QVERIFY(config.enabled);
    QVERIFY(!config.useImapCustomServer);
}

void SieveEditorUtilTest::shouldTestSieveAccountSettingsDefaultValue()
{
    SieveEditorUtil::SieveAccountSettings settings;
    QVERIFY(settings.serverName.isEmpty());
    QVERIFY(settings.userName.isEmpty());
    QVERIFY(settings.password.isEmpty());
    QCOMPARE(settings.authenticationType, MailTransport::Transport::EnumAuthenticationType::PLAIN);
    QCOMPARE(settings.port, -1);
    QVERIFY(!settings.isValid());
}

void SieveEditorUtilTest::shouldSieveAccountSettingsMustBeValid()
{
    SieveEditorUtil::SieveAccountSettings settings;
    QVERIFY(!settings.isValid());
    settings.serverName = QStringLiteral("foo");
    QVERIFY(!settings.isValid());
    settings.userName = QStringLiteral("bla");
    QVERIFY(!settings.isValid());
    settings.password = QStringLiteral("bli");
    QVERIFY(!settings.isValid());
    settings.port = 2;
    QVERIFY(settings.isValid());
}

void SieveEditorUtilTest::shouldBeEqual()
{
    SieveEditorUtil::SieveAccountSettings settings;
    settings.serverName = QStringLiteral("foo");
    settings.userName = QStringLiteral("bla");
    settings.password = QStringLiteral("bli");
    settings.port = 2;
    SieveEditorUtil::SieveAccountSettings settings2 = settings;
    QVERIFY(settings == settings2);

    SieveEditorUtil::SieveAccountSettings settings3;
    settings3.serverName = QStringLiteral("foo");
    settings3.userName = QStringLiteral("bla");
    settings3.password = QStringLiteral("bli");
    settings3.port = 3;
    QVERIFY(!(settings == settings3));

    settings3.port = 2;
    QVERIFY(settings == settings3);
}

QTEST_APPLESS_MAIN(SieveEditorUtilTest)

#include "moc_sieveeditorutiltest.cpp"
