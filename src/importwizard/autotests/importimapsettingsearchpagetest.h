/*
   SPDX-FileCopyrightText: 2016-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef IMPORTIMAPSETTINGSEARCHPAGETEST_H
#define IMPORTIMAPSETTINGSEARCHPAGETEST_H

#include <QObject>

class ImportImapSettingSearchPageTest : public QObject
{
    Q_OBJECT
public:
    explicit ImportImapSettingSearchPageTest(QObject *parent = nullptr);
    ~ImportImapSettingSearchPageTest();
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldAddCheckPrograms();
};

#endif // IMPORTIMAPSETTINGSEARCHPAGETEST_H
