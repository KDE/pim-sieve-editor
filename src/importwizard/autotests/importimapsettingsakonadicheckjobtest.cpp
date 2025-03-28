/*
   SPDX-FileCopyrightText: 2016-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "importimapsettingsakonadicheckjobtest.h"
#include "../checks/importimapsettingsakonadicheckjob.h"
#include "importimapsettingspasswordtest.h"

#include <QSignalSpy>
#include <QStandardPaths>
#include <QTest>

// taken from tst_qstandardpaths
#if defined(Q_OS_UNIX) && !defined(Q_OS_MAC) && !defined(Q_OS_BLACKBERRY) && !defined(Q_OS_ANDROID)
#define Q_XDG_PLATFORM
#endif

Q_DECLARE_METATYPE(SieveEditorUtil::SieveServerConfig)
ImportImapSettingsAkonadiCheckJobTest::ImportImapSettingsAkonadiCheckJobTest(QObject *parent)
    : QObject(parent)
{
    qRegisterMetaType<SieveEditorUtil::SieveServerConfig>();
}

ImportImapSettingsAkonadiCheckJobTest::~ImportImapSettingsAkonadiCheckJobTest() = default;

void ImportImapSettingsAkonadiCheckJobTest::initTestCase()
{
#ifndef Q_XDG_PLATFORM
    // We need to make changes to a global dir without messing up the system
    QSKIP("This test requires XDG_DATA_DIRS");
#endif

    QStandardPaths::setTestModeEnabled(true);
}

void ImportImapSettingsAkonadiCheckJobTest::shouldHaveSettingsFiles()
{
    qputenv("XDG_CONFIG_DIRS", QString(QLatin1StringView(IMPORTWIZARD_DATA_DIR) + QStringLiteral("/config/config1")).toLatin1().constData());
    ImportImapSettingsAkonadiCheckJob job;
    job.setImapSettingsPassword(new ImportImapSettingsPasswordTest);
    QVERIFY(job.settingsCanBeImported());
}

void ImportImapSettingsAkonadiCheckJobTest::shouldHaveNotSettingsFiles()
{
    qputenv("XDG_CONFIG_DIRS", QString(QLatin1StringView(IMPORTWIZARD_DATA_DIR) + QStringLiteral("/config/noconfig")).toLatin1().constData());
    ImportImapSettingsAkonadiCheckJob job;
    job.setImapSettingsPassword(new ImportImapSettingsPasswordTest);
    QVERIFY(!job.settingsCanBeImported());
}

void ImportImapSettingsAkonadiCheckJobTest::shouldHaveSettingsFilesImap()
{
    qputenv("XDG_CONFIG_DIRS", QString(QLatin1StringView(IMPORTWIZARD_DATA_DIR) + QStringLiteral("/config/config2")).toLatin1().constData());
    ImportImapSettingsAkonadiCheckJob job;
    job.setImapSettingsPassword(new ImportImapSettingsPasswordTest);
    QVERIFY(job.settingsCanBeImported());
}

void ImportImapSettingsAkonadiCheckJobTest::shouldHaveSettingsFilesMbox()
{
    qputenv("XDG_CONFIG_DIRS", QString(QLatin1StringView(IMPORTWIZARD_DATA_DIR) + QStringLiteral("/config/config3")).toLatin1().constData());
    ImportImapSettingsAkonadiCheckJob job;
    job.setImapSettingsPassword(new ImportImapSettingsPasswordTest);
    QVERIFY(!job.settingsCanBeImported());
}

void ImportImapSettingsAkonadiCheckJobTest::shouldHaveImportSettings_data()
{
    QTest::addColumn<QString>("directory");
    QTest::addColumn<int>("nbsignals");
    QTest::addColumn<int>("nbSignalsNoSettingsFound");
    QTest::newRow("reuseconfig") << QStringLiteral("/config/reuseconfig") << 1 << 0;
    QTest::newRow("reuseconfigtwiceconfig") << QStringLiteral("/config/reuseconfigtwiceconfig") << 2 << 0;

    QTest::newRow("imapconfig") << QStringLiteral("/config/imapconfig") << 0 << 1;
    QTest::newRow("imapconfigwithsieveconfig") << QStringLiteral("/config/imapconfigwithsieveconfig") << 1 << 0;
}

void ImportImapSettingsAkonadiCheckJobTest::shouldHaveImportSettings()
{
    QFETCH(QString, directory);
    QFETCH(int, nbsignals);
    QFETCH(int, nbSignalsNoSettingsFound);
    qputenv("XDG_CONFIG_DIRS", QString(QLatin1StringView(IMPORTWIZARD_DATA_DIR) + directory).toLatin1().constData());
    ImportImapSettingsAkonadiCheckJob job;
    job.setImapSettingsPassword(new ImportImapSettingsPasswordTest);
    QVERIFY(job.settingsCanBeImported());
    QSignalSpy spy(&job, &ImportImapSettingsAkonadiCheckJob::importSetting);
    QSignalSpy spy2(&job, &ImportImapSettingsAkonadiCheckJob::noSettingsImported);

    job.start();
    QCOMPARE(spy.count(), nbsignals);
    QCOMPARE(spy2.count(), nbSignalsNoSettingsFound);
}

void ImportImapSettingsAkonadiCheckJobTest::shouldImportSieveSettings_data()
{
    QTest::addColumn<QString>("directory");
    QTest::addColumn<QString>("name");
    QTest::addColumn<SieveEditorUtil::SieveServerConfig>("settings");

    SieveEditorUtil::SieveServerConfig settings;
    settings.sieveImapAccountSettings.setUserName(QStringLiteral("foo@foo.com"));
    settings.sieveImapAccountSettings.setServerName(QStringLiteral("bla.foo.com"));
    settings.sieveImapAccountSettings.setPort(143);
    settings.sieveImapAccountSettings.setPassword(QStringLiteral("password_imap"));
    settings.sieveImapAccountSettings.setEncryptionMode(KSieveCore::SieveImapAccountSettings::EncryptionMode::STARTTLS);

    settings.sieveSettings.userName = QStringLiteral("foo@foo.com");
    settings.sieveSettings.serverName = QStringLiteral("bla.foo.com");
    settings.sieveSettings.password = QStringLiteral("password_imap");
    settings.sieveSettings.port = 4190;

    QTest::newRow("reuseconfig") << QStringLiteral("/config/reuseconfig") << QStringLiteral("akonadi_kolab_resource_0rc") << settings;

    SieveEditorUtil::SieveServerConfig settings2;
    settings2.sieveImapAccountSettings.setUserName(QStringLiteral("foo@foo.com"));
    settings2.sieveImapAccountSettings.setServerName(QStringLiteral("bla.foo.com"));
    settings2.sieveImapAccountSettings.setPort(143);
    settings2.sieveImapAccountSettings.setPassword(QStringLiteral("password_imap"));
    settings2.sieveImapAccountSettings.setEncryptionMode(KSieveCore::SieveImapAccountSettings::EncryptionMode::STARTTLS);
    settings2.sieveSettings.userName = QStringLiteral("b");
    settings2.sieveSettings.serverName = QStringLiteral("bla.foo.com"); // TODO
    settings2.sieveSettings.password = QStringLiteral("password_custom_sieve_imap");
    settings2.sieveSettings.port = 150;

    QTest::newRow("usecustomsieveconfig") << QStringLiteral("/config/usecustomsieveconfig") << QStringLiteral("akonadi_kolab_resource_0rc") << settings2;

    SieveEditorUtil::SieveServerConfig settings3;
    settings3.sieveImapAccountSettings.setUserName(QStringLiteral("foo@foo.com"));
    settings3.sieveImapAccountSettings.setServerName(QStringLiteral("bla.foo.com"));
    settings3.sieveImapAccountSettings.setPort(143);
    settings3.sieveImapAccountSettings.setAuthenticationType(static_cast<KSieveCore::SieveImapAccountSettings::AuthenticationMode>(3));
    settings3.sieveImapAccountSettings.setEncryptionMode(KSieveCore::SieveImapAccountSettings::EncryptionMode::Unencrypted);
    settings3.sieveImapAccountSettings.setPassword(QStringLiteral("password_imap"));
    settings3.sieveSettings.userName = QStringLiteral("b");
    settings3.sieveSettings.password = QStringLiteral("password_custom_sieve_imap");
    settings3.sieveSettings.serverName = QStringLiteral("bla.foo.com"); // TODO
    settings3.sieveSettings.port = 150;

    QTest::newRow("usecustomsieveconfig2") << QStringLiteral("/config/usecustomsieveconfig2") << QStringLiteral("akonadi_kolab_resource_0rc") << settings3;

    SieveEditorUtil::SieveServerConfig settings4;
    settings4.sieveImapAccountSettings.setUserName(QStringLiteral("foo@foo.com"));
    settings4.sieveImapAccountSettings.setServerName(QStringLiteral("bla.foo.com"));
    settings4.sieveImapAccountSettings.setPort(143);
    settings4.sieveImapAccountSettings.setAuthenticationType(static_cast<KSieveCore::SieveImapAccountSettings::AuthenticationMode>(3));
    settings4.sieveImapAccountSettings.setEncryptionMode(KSieveCore::SieveImapAccountSettings::EncryptionMode::STARTTLS);
    settings4.sieveImapAccountSettings.setPassword(QStringLiteral("password_imap"));
    settings4.sieveSettings.userName = QStringLiteral("b");
    settings4.sieveSettings.password = QStringLiteral("password_custom_sieve_imap");
    settings4.sieveSettings.serverName = QStringLiteral("bla.foo.com"); // TODO
    settings4.sieveSettings.port = 150;

    QTest::newRow("usecustomsieveconfig3") << QStringLiteral("/config/usecustomsieveconfig3") << QStringLiteral("akonadi_kolab_resource_0rc") << settings4;

    SieveEditorUtil::SieveServerConfig settings5;
    settings5.sieveImapAccountSettings.setUserName(QStringLiteral("foo@foo.com"));
    settings5.sieveImapAccountSettings.setServerName(QStringLiteral("bla.foo.com"));
    settings5.sieveImapAccountSettings.setPort(143);
    settings5.sieveImapAccountSettings.setAuthenticationType(static_cast<KSieveCore::SieveImapAccountSettings::AuthenticationMode>(3));
    settings5.sieveImapAccountSettings.setEncryptionMode(KSieveCore::SieveImapAccountSettings::EncryptionMode::SSLorTLS);
    settings5.sieveImapAccountSettings.setPassword(QStringLiteral("password_imap"));
    settings5.sieveSettings.userName = QStringLiteral("b");
    settings5.sieveSettings.serverName = QStringLiteral("bla.foo.com"); // TODO
    settings5.sieveSettings.password = QStringLiteral("password_custom_sieve_imap");
    settings5.sieveSettings.port = 150;

    QTest::newRow("usecustomsieveconfig4") << QStringLiteral("/config/usecustomsieveconfig4") << QStringLiteral("akonadi_kolab_resource_0rc") << settings5;
}

void ImportImapSettingsAkonadiCheckJobTest::shouldImportSieveSettings()
{
    QFETCH(QString, directory);
    QFETCH(QString, name);
    QFETCH(SieveEditorUtil::SieveServerConfig, settings);
    qputenv("XDG_CONFIG_DIRS", QString(QLatin1StringView(IMPORTWIZARD_DATA_DIR) + directory).toLatin1().constData());
    ImportImapSettingsAkonadiCheckJob job;
    job.setImapSettingsPassword(new ImportImapSettingsPasswordTest);
    QVERIFY(job.settingsCanBeImported());
    QSignalSpy spy(&job, &ImportImapSettingsAkonadiCheckJob::importSetting);
    job.start();
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).toString(), name);
    auto importSettings = spy.at(0).at(1).value<SieveEditorUtil::SieveServerConfig>();
    QCOMPARE(importSettings, settings);
}

QTEST_MAIN(ImportImapSettingsAkonadiCheckJobTest)

#include "moc_importimapsettingsakonadicheckjobtest.cpp"
