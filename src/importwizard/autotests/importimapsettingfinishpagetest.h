/*
   SPDX-FileCopyrightText: 2016-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef IMPORTIMAPSETTINGFINISHPAGETEST_H
#define IMPORTIMAPSETTINGFINISHPAGETEST_H

#include <QObject>

class ImportImapSettingFinishPageTest : public QObject
{
    Q_OBJECT
public:
    explicit ImportImapSettingFinishPageTest(QObject *parent = nullptr);
    ~ImportImapSettingFinishPageTest();
private Q_SLOTS:
    void shouldHaveDefaultValue();
};

#endif // IMPORTIMAPSETTINGFINISHPAGETEST_H
