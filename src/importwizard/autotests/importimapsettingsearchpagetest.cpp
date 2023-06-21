/*
   SPDX-FileCopyrightText: 2016-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "importimapsettingsearchpagetest.h"
#include "../importimapsettingsearchpage.h"
#include <QLabel>
#include <QListWidget>
#include <QStandardPaths>
#include <QTest>
#include <QVBoxLayout>
ImportImapSettingSearchPageTest::ImportImapSettingSearchPageTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
}

ImportImapSettingSearchPageTest::~ImportImapSettingSearchPageTest() = default;

void ImportImapSettingSearchPageTest::shouldHaveDefaultValue()
{
    ImportImapSettingSearchPage w;
    auto label = w.findChild<QLabel *>(QStringLiteral("label"));
    QVERIFY(label);
    QVERIFY(!label->text().isEmpty());

    auto mFoundProgramList = w.findChild<QListWidget *>(QStringLiteral("foundprogramlist"));
    QVERIFY(mFoundProgramList);
    QCOMPARE(mFoundProgramList->count(), 0);

    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainlayout"));
    QVERIFY(mainLayout);
}

void ImportImapSettingSearchPageTest::shouldAddCheckPrograms()
{
    ImportImapSettingSearchPage w;
    const QStringList lst{QStringLiteral("foo"), QStringLiteral("bla"), QStringLiteral("do")};

    w.setProgramList(lst);
    auto mFoundProgramList = w.findChild<QListWidget *>(QStringLiteral("foundprogramlist"));
    QCOMPARE(mFoundProgramList->count(), lst.count());
    for (int i = 0; i < mFoundProgramList->count(); ++i) {
        QCOMPARE(mFoundProgramList->item(i)->checkState(), Qt::Unchecked);
    }
}

QTEST_MAIN(ImportImapSettingSearchPageTest)

#include "moc_importimapsettingsearchpagetest.cpp"
