/*
   SPDX-FileCopyrightText: 2016-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "importimapsettingnofoundpagetest.h"
#include "../importimapsettingnofoundpage.h"
#include <QLabel>
#include <QTest>
#include <QVBoxLayout>

ImportImapSettingNoFoundPageTest::ImportImapSettingNoFoundPageTest(QObject *parent)
    : QObject(parent)
{
}

ImportImapSettingNoFoundPageTest::~ImportImapSettingNoFoundPageTest()
{
}

void ImportImapSettingNoFoundPageTest::shouldHaveDefaultValue()
{
    ImportImapSettingNoFoundPage w;
    QLabel *lab = w.findChild<QLabel *>(QStringLiteral("label"));
    QVERIFY(lab);
    QVERIFY(!lab->text().isEmpty());

    QVBoxLayout *mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainlayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins(0, 0, 0, 0));
}

QTEST_MAIN(ImportImapSettingNoFoundPageTest)
