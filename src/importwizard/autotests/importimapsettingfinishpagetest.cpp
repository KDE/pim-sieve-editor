/*
   Copyright (C) 2016-2021 Laurent Montel <montel@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
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
