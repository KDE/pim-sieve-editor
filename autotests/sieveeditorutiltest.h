/*
   SPDX-FileCopyrightText: 2014-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef SIEVEEDITORUTILTEST_H
#define SIEVEEDITORUTILTEST_H

#include <QObject>

class SieveEditorUtilTest : public QObject
{
    Q_OBJECT
public:
    explicit SieveEditorUtilTest(QObject *parent = nullptr);
    ~SieveEditorUtilTest();
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldTestSieveAccountSettingsDefaultValue();
    void shouldSieveAccountSettingsMustBeValid();
    void shouldBeEqual();
};

#endif // SIEVEEDITORUTILTEST_H
