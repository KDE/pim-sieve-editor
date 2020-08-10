/*
   SPDX-FileCopyrightText: 2016-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "importimapsettingprogresspagetest.h"
#include "../importimapsettingprogresspage.h"
#include <QTest>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QStandardPaths>

ImportImapSettingProgressPageTest::ImportImapSettingProgressPageTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
}

ImportImapSettingProgressPageTest::~ImportImapSettingProgressPageTest()
{
}

void ImportImapSettingProgressPageTest::shouldHaveDefaultValue()
{
    ImportImapSettingProgressPage w;

    QTextEdit *mProgressTextEdit = w.findChild<QTextEdit *>(QStringLiteral("textedit"));
    QVERIFY(mProgressTextEdit);
    QVERIFY(mProgressTextEdit->isReadOnly());
    QVERIFY(mProgressTextEdit->document()->isEmpty());

    QVBoxLayout *mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainlayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins(0, 0, 0, 0));
}

QTEST_MAIN(ImportImapSettingProgressPageTest)
