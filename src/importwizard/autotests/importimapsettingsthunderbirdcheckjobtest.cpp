/*
   Copyright (C) 2017 Montel Laurent <montel@kde.org>

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

#include "importimapsettingsthunderbirdcheckjobtest.h"
#include "importwizard/checks/importimapsettingsthunderbirdcheckjob.h"
#include "libsieveeditor_export.h"
#include <QTest>
#include <QStandardPaths>
#include <QSignalSpy>

Q_DECLARE_METATYPE(SieveEditorUtil::SieveServerConfig)
extern LIBSIEVEEDITOR_EXPORT QString sieveeditor_thunderbird_default_toplevel_path;
ImportImapSettingsThunderbirdCheckJobTest::ImportImapSettingsThunderbirdCheckJobTest(QObject *parent)
    : QObject(parent)
{
    qRegisterMetaType<SieveEditorUtil::SieveServerConfig>();
    QStandardPaths::setTestModeEnabled(true);
    sieveeditor_thunderbird_default_toplevel_path = QString();
}

ImportImapSettingsThunderbirdCheckJobTest::~ImportImapSettingsThunderbirdCheckJobTest()
{

}

void ImportImapSettingsThunderbirdCheckJobTest::shouldHaveDefaultValue()
{
    sieveeditor_thunderbird_default_toplevel_path = QString(QLatin1String(IMPORTWIZARD_DATA_DIR) + QStringLiteral("/empty2"));
    ImportImapSettingsThunderbirdCheckJob check;
    QVERIFY(!check.settingsCanBeImported());
}

void ImportImapSettingsThunderbirdCheckJobTest::shouldHaveDefaultValueWithEmptyDirectory()
{
    sieveeditor_thunderbird_default_toplevel_path = QString(QLatin1String(IMPORTWIZARD_DATA_DIR) + QStringLiteral("/empty"));
    ImportImapSettingsThunderbirdCheckJob check;
    QVERIFY(check.settingsCanBeImported());
    check.start();
}

void ImportImapSettingsThunderbirdCheckJobTest::shouldBeAbleToImportSettings()
{
    sieveeditor_thunderbird_default_toplevel_path = QString(QLatin1String(IMPORTWIZARD_DATA_DIR) + QStringLiteral("/thunderbird1"));
    ImportImapSettingsThunderbirdCheckJob check;
    QVERIFY(check.settingsCanBeImported());
    check.start();
}

void ImportImapSettingsThunderbirdCheckJobTest::shouldBeAbleToImportSettings2()
{
    sieveeditor_thunderbird_default_toplevel_path = QString(QLatin1String(IMPORTWIZARD_DATA_DIR) + QStringLiteral("/thunderbird2"));
    ImportImapSettingsThunderbirdCheckJob check;
    QVERIFY(check.settingsCanBeImported());
    check.start();
}

void ImportImapSettingsThunderbirdCheckJobTest::shouldHaveImportSettings_data()
{
    QTest::addColumn<QString>("directory");
    QTest::addColumn<int>("nbsignals");
    QTest::addColumn<int>("nbSignalsNoSettingsFound");
    QTest::newRow("thunderbird1-withoutsieve") << QStringLiteral("/thunderbird1") << 0 << 1;
    QTest::newRow("empty") << QStringLiteral("/empty") << 0 << 1;
    QTest::newRow("thunderbird2-withoutsieve") << QStringLiteral("/thunderbird2") << 0 << 1;
}

void ImportImapSettingsThunderbirdCheckJobTest::shouldHaveImportSettings()
{
    QFETCH(QString, directory);
    QFETCH(int, nbsignals);
    QFETCH(int, nbSignalsNoSettingsFound);
    sieveeditor_thunderbird_default_toplevel_path = QString(QLatin1String(IMPORTWIZARD_DATA_DIR) + directory);
    ImportImapSettingsThunderbirdCheckJob job;
    QVERIFY(job.settingsCanBeImported());
    QSignalSpy spy(&job, &ImportImapSettingsThunderbirdCheckJob::importSetting);
    QSignalSpy spy2(&job, &ImportImapSettingsThunderbirdCheckJob::noSettingsImported);

    job.start();
    QCOMPARE(spy.count(), nbsignals);
    QCOMPARE(spy2.count(), nbSignalsNoSettingsFound);
}

QTEST_MAIN(ImportImapSettingsThunderbirdCheckJobTest)
