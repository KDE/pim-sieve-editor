/*
   SPDX-FileCopyrightText: 2016-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef IMPORTIMAPSETTINGNOFOUNDPAGETEST_H
#define IMPORTIMAPSETTINGNOFOUNDPAGETEST_H

#include <QObject>

class ImportImapSettingNoFoundPageTest : public QObject
{
    Q_OBJECT
public:
    explicit ImportImapSettingNoFoundPageTest(QObject *parent = nullptr);
    ~ImportImapSettingNoFoundPageTest();
private Q_SLOTS:
    void shouldHaveDefaultValue();
};

#endif // IMPORTIMAPSETTINGNOFOUNDPAGETEST_H
