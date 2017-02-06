/*
   Copyright (C) 2016-2017 Montel Laurent <montel@kde.org>

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

#include "importimapsettingsakonadicheckjobtest.h"
#include "../checks/importimapsettingsakonadicheckjob.h"

#include <QTest>
#include <QStandardPaths>
#include <QDebug>
#include <QSignalSpy>

Q_DECLARE_METATYPE(SieveEditorUtil::SieveServerConfig)
ImportImapSettingsAkonadiCheckJobTest::ImportImapSettingsAkonadiCheckJobTest(QObject *parent)
    : QObject(parent)
{
    qRegisterMetaType<SieveEditorUtil::SieveServerConfig>();
}

ImportImapSettingsAkonadiCheckJobTest::~ImportImapSettingsAkonadiCheckJobTest()
{

}

void ImportImapSettingsAkonadiCheckJobTest::initTestCase()
{
    QStandardPaths::setTestModeEnabled(true);
}

void ImportImapSettingsAkonadiCheckJobTest::shouldHaveSettingsFiles()
{
    qputenv("XDG_CONFIG_DIRS", QString(QLatin1String(IMPORTWIZARD_DATA_DIR) + QStringLiteral("/config/config1")).toLatin1().constData());
    ImportImapSettingsAkonadiCheckJob job;
    QVERIFY(job.settingsCanBeImported());
}

void ImportImapSettingsAkonadiCheckJobTest::shouldHaveNotSettingsFiles()
{
    qputenv("XDG_CONFIG_DIRS", QString(QLatin1String(IMPORTWIZARD_DATA_DIR) + QStringLiteral("/config/noconfig")).toLatin1().constData());
    ImportImapSettingsAkonadiCheckJob job;
    QVERIFY(!job.settingsCanBeImported());
}

void ImportImapSettingsAkonadiCheckJobTest::shouldHaveSettingsFilesImap()
{
    qputenv("XDG_CONFIG_DIRS", QString(QLatin1String(IMPORTWIZARD_DATA_DIR) + QStringLiteral("/config/config2")).toLatin1().constData());
    ImportImapSettingsAkonadiCheckJob job;
    QVERIFY(job.settingsCanBeImported());
}

void ImportImapSettingsAkonadiCheckJobTest::shouldHaveSettingsFilesMbox()
{
    qputenv("XDG_CONFIG_DIRS", QString(QLatin1String(IMPORTWIZARD_DATA_DIR) + QStringLiteral("/config/config3")).toLatin1().constData());
    ImportImapSettingsAkonadiCheckJob job;
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
    qputenv("XDG_CONFIG_DIRS", QString(QLatin1String(IMPORTWIZARD_DATA_DIR) + directory).toLatin1().constData());
    ImportImapSettingsAkonadiCheckJob job;
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
    settings.sieveSettings.userName = QStringLiteral("foo@foo.com");
    settings.sieveSettings.serverName = QStringLiteral("bla.foo.com");
    settings.sieveSettings.port = 4190;

    QTest::newRow("reuseconfig") << QStringLiteral("/config/reuseconfig") << QStringLiteral("akonadi_kolab_resource_0rc") <<settings;

    SieveEditorUtil::SieveServerConfig settings2;
    settings2.sieveImapAccountSettings.setUserName(QStringLiteral("foo@foo.com"));
    settings2.sieveImapAccountSettings.setServerName(QStringLiteral("bla.foo.com"));
    settings2.sieveImapAccountSettings.setPort(143);
    settings2.sieveSettings.userName = QStringLiteral("b");
    settings2.sieveSettings.serverName = QStringLiteral("bla.foo.com"); //TODO
    settings2.sieveSettings.port = 150;

    QTest::newRow("usecustomsieveconfig") << QStringLiteral("/config/usecustomsieveconfig") << QStringLiteral("akonadi_kolab_resource_0rc") <<settings2;

    SieveEditorUtil::SieveServerConfig settings3;
    settings3.sieveImapAccountSettings.setUserName(QStringLiteral("foo@foo.com"));
    settings3.sieveImapAccountSettings.setServerName(QStringLiteral("bla.foo.com"));
    settings3.sieveImapAccountSettings.setPort(143);
    settings3.sieveSettings.userName = QStringLiteral("b");
    settings3.sieveSettings.serverName = QStringLiteral("bla.foo.com"); //TODO
    settings3.sieveSettings.port = 150;

    QTest::newRow("usecustomsieveconfig2") << QStringLiteral("/config/usecustomsieveconfig2") << QStringLiteral("akonadi_kolab_resource_0rc") <<settings3;

}

void ImportImapSettingsAkonadiCheckJobTest::shouldImportSieveSettings()
{
    QFETCH(QString, directory);
    QFETCH(QString, name);
    QFETCH(SieveEditorUtil::SieveServerConfig, settings);
    qputenv("XDG_CONFIG_DIRS", QString(QLatin1String(IMPORTWIZARD_DATA_DIR) + directory).toLatin1().constData());
    ImportImapSettingsAkonadiCheckJob job;
    QVERIFY(job.settingsCanBeImported());
    QSignalSpy spy(&job, &ImportImapSettingsAkonadiCheckJob::importSetting);
    job.start();
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).toString(), name);
    SieveEditorUtil::SieveServerConfig importSettings = spy.at(0).at(1).value<SieveEditorUtil::SieveServerConfig>();
    QCOMPARE(importSettings, settings);
}

QTEST_MAIN(ImportImapSettingsAkonadiCheckJobTest)
