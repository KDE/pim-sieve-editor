/*
   SPDX-FileCopyrightText: 2016-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "importimapsettingsearchpagetest.h"
#include "../importimapsettingsearchpage.h"
#include <QLabel>
#include <QListWidget>
#include <QTest>
#include <QVBoxLayout>
#include <QStandardPaths>
ImportImapSettingSearchPageTest::ImportImapSettingSearchPageTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
}

ImportImapSettingSearchPageTest::~ImportImapSettingSearchPageTest()
{
}

void ImportImapSettingSearchPageTest::shouldHaveDefaultValue()
{
    ImportImapSettingSearchPage w;
    QLabel *label = w.findChild<QLabel *>(QStringLiteral("label"));
    QVERIFY(label);
    QVERIFY(!label->text().isEmpty());

    QListWidget *mFoundProgramList = w.findChild<QListWidget *>(QStringLiteral("foundprogramlist"));
    QVERIFY(mFoundProgramList);
    QCOMPARE(mFoundProgramList->count(), 0);

    QVBoxLayout *mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainlayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins(0, 0, 0, 0));
}

void ImportImapSettingSearchPageTest::shouldAddCheckPrograms()
{
    ImportImapSettingSearchPage w;
    const QStringList lst {
        QStringLiteral("foo"), QStringLiteral("bla"), QStringLiteral("do")
    };

    w.setProgramList(lst);
    QListWidget *mFoundProgramList = w.findChild<QListWidget *>(QStringLiteral("foundprogramlist"));
    QCOMPARE(mFoundProgramList->count(), lst.count());
    for (int i = 0; i < mFoundProgramList->count(); ++i) {
        QCOMPARE(mFoundProgramList->item(i)->checkState(), Qt::Unchecked);
    }
}

QTEST_MAIN(ImportImapSettingSearchPageTest)
