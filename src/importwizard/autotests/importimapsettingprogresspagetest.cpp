/*
   SPDX-FileCopyrightText: 2016-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "importimapsettingprogresspagetest.h"
#include "../importimapsettingprogresspage.h"
#include <QStandardPaths>
#include <QTest>
#include <QTextEdit>
#include <QVBoxLayout>

ImportImapSettingProgressPageTest::ImportImapSettingProgressPageTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
}

ImportImapSettingProgressPageTest::~ImportImapSettingProgressPageTest() = default;

void ImportImapSettingProgressPageTest::shouldHaveDefaultValue()
{
    ImportImapSettingProgressPage w;

    auto mProgressTextEdit = w.findChild<QTextEdit *>(QStringLiteral("textedit"));
    QVERIFY(mProgressTextEdit);
    QVERIFY(mProgressTextEdit->isReadOnly());
    QVERIFY(mProgressTextEdit->document()->isEmpty());

    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainlayout"));
    QVERIFY(mainLayout);
}

QTEST_MAIN(ImportImapSettingProgressPageTest)
