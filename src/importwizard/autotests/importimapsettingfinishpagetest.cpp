/*
   SPDX-FileCopyrightText: 2016-2023 Laurent Montel <montel@kde.org>

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

ImportImapSettingFinishPageTest::~ImportImapSettingFinishPageTest() = default;

void ImportImapSettingFinishPageTest::shouldHaveDefaultValue()
{
    ImportImapSettingFinishPage w;
    auto label = w.findChild<QLabel *>(QStringLiteral("label"));
    QVERIFY(label);
    QVERIFY(!label->text().isEmpty());

    auto mTextEdit = w.findChild<QTextEdit *>(QStringLiteral("textedit"));
    QVERIFY(mTextEdit);
    QVERIFY(mTextEdit->isReadOnly());
    QVERIFY(mTextEdit->document()->isEmpty());

    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainlayout"));
    QVERIFY(mainLayout);
}

QTEST_MAIN(ImportImapSettingFinishPageTest)
