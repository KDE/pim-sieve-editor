/*
   SPDX-FileCopyrightText: 2016-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef IMPORTIMAPSETTINGSAKONADICHECKJOBTEST_H
#define IMPORTIMAPSETTINGSAKONADICHECKJOBTEST_H

#include <QObject>

class ImportImapSettingsAkonadiCheckJobTest : public QObject
{
    Q_OBJECT
public:
    explicit ImportImapSettingsAkonadiCheckJobTest(QObject *parent = nullptr);
    ~ImportImapSettingsAkonadiCheckJobTest();
private Q_SLOTS:
    void initTestCase();
    void shouldHaveSettingsFiles();
    void shouldHaveNotSettingsFiles();
    void shouldHaveSettingsFilesImap();
    void shouldHaveSettingsFilesMbox();

    void shouldHaveImportSettings_data();
    void shouldHaveImportSettings();

    void shouldImportSieveSettings_data();
    void shouldImportSieveSettings();
};

#endif // IMPORTIMAPSETTINGSAKONADICHECKJOBTEST_H
