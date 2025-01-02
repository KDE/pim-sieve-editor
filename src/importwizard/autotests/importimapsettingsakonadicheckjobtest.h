/*
   SPDX-FileCopyrightText: 2016-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class ImportImapSettingsAkonadiCheckJobTest : public QObject
{
    Q_OBJECT
public:
    explicit ImportImapSettingsAkonadiCheckJobTest(QObject *parent = nullptr);
    ~ImportImapSettingsAkonadiCheckJobTest() override;
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
