/*
   SPDX-FileCopyrightText: 2014-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class SieveEditorUtilTest : public QObject
{
    Q_OBJECT
public:
    explicit SieveEditorUtilTest(QObject *parent = nullptr);
    ~SieveEditorUtilTest() override;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldTestSieveAccountSettingsDefaultValue();
    void shouldSieveAccountSettingsMustBeValid();
    void shouldBeEqual();
};

