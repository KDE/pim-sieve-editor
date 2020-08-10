/*
   SPDX-FileCopyrightText: 2016-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "importimapsettingfinishpagetest.h"
#include "../importimapsettingfinishpage.h"
#include <QLabel>
#include <QTest>
#include <QTextEdit>
#include <QVBoxLayout>

ImportImapSettingFinishPageTest::ImportImapSettingFinishPageTest(QObject *parent)
    : QObject(parent)
{
}

ImportImapSettingFinishPageTest::~ImportImapSettingFinishPageTest()
{
}

void ImportImapSettingFinishPageTest::shouldHaveDefaultValue()
{
    ImportImapSettingFinishPage w;
    QLabel *label = w.findChild<QLabel *>(QStringLiteral("label"));
    QVERIFY(label);
    QVERIFY(!label->text().isEmpty());

    QTextEdit *mTextEdit = w.findChild<QTextEdit *>(QStringLiteral("textedit"));
    QVERIFY(mTextEdit);
    QVERIFY(mTextEdit->isReadOnly());
    QVERIFY(mTextEdit->document()->isEmpty());

    QVBoxLayout *mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainlayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins(0, 0, 0, 0));
}

QTEST_MAIN(ImportImapSettingFinishPageTest)
