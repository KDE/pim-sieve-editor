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
    QTest::newRow("reuseconfig") << QStringLiteral("/config/reuseconfig") << 1;
    QTest::newRow("reuseconfigtwiceconfig") << QStringLiteral("/config/reuseconfigtwiceconfig") << 2;

    QTest::newRow("imapconfig") << QStringLiteral("/config/imapconfig") << 0;
    QTest::newRow("imapconfigwithsieveconfig") << QStringLiteral("/config/imapconfigwithsieveconfig") << 1;
}

void ImportImapSettingsAkonadiCheckJobTest::shouldHaveImportSettings()
{
    QFETCH(QString, directory);
    QFETCH(int, nbsignals);
    qputenv("XDG_CONFIG_DIRS", QString(QLatin1String(IMPORTWIZARD_DATA_DIR) + directory).toLatin1().constData());
    ImportImapSettingsAkonadiCheckJob job;
    QVERIFY(job.settingsCanBeImported());
    QSignalSpy spy(&job, &ImportImapSettingsAkonadiCheckJob::importSetting);
    job.start();
    QCOMPARE(spy.count(), nbsignals);
}

void ImportImapSettingsAkonadiCheckJobTest::shouldImportSieveSettings_data()
{
    QTest::addColumn<QString>("directory");
    QTest::addColumn<SieveEditorUtil::SieveServerConfig>("settings");
    QTest::newRow("reuseconfig") << QStringLiteral("/config/reuseconfig") << SieveEditorUtil::SieveServerConfig();
}

void ImportImapSettingsAkonadiCheckJobTest::shouldImportSieveSettings()
{
    QFETCH(QString, directory);
    QFETCH(SieveEditorUtil::SieveServerConfig, settings);
    qputenv("XDG_CONFIG_DIRS", QString(QLatin1String(IMPORTWIZARD_DATA_DIR) + directory).toLatin1().constData());
}

QTEST_MAIN(ImportImapSettingsAkonadiCheckJobTest)
