/*
   SPDX-FileCopyrightText: 2016-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef IMPORTIMAPSETTINGPROGRESSPAGETEST_H
#define IMPORTIMAPSETTINGPROGRESSPAGETEST_H

#include <QObject>

class ImportImapSettingProgressPageTest : public QObject
{
    Q_OBJECT
public:
    explicit ImportImapSettingProgressPageTest(QObject *parent = nullptr);
    ~ImportImapSettingProgressPageTest();
private Q_SLOTS:
    void shouldHaveDefaultValue();
};

#endif // IMPORTIMAPSETTINGPROGRESSPAGETEST_H
