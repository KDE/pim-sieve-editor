/*
   SPDX-FileCopyrightText: 2016-2026 Laurent Montel <montel@kde.org>

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

ImportImapSettingNoFoundPageTest::~ImportImapSettingNoFoundPageTest() = default;

void ImportImapSettingNoFoundPageTest::shouldHaveDefaultValue()
{
    ImportImapSettingNoFoundPage w;
    auto lab = w.findChild<QLabel *>(QStringLiteral("label"));
    QVERIFY(lab);
    QVERIFY(!lab->text().isEmpty());

    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainlayout"));
    QVERIFY(mainLayout);
}

QTEST_MAIN(ImportImapSettingNoFoundPageTest)

#include "moc_importimapsettingnofoundpagetest.cpp"
