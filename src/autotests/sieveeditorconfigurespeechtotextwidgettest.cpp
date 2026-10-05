/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "sieveeditorconfigurespeechtotextwidgettest.h"

#include "sieveeditorconfigurespeechtotextwidget.h"
#include <QCheckBox>
#include <QStandardPaths>
#include <QTest>
#include <QVBoxLayout>
#include <TextSpeechToText/SpeechToTextConfigureWidget>

using namespace Qt::Literals::StringLiterals;

QTEST_MAIN(SieveEditorConfigureSpeechToTextWidgetTest)
SieveEditorConfigureSpeechToTextWidgetTest::SieveEditorConfigureSpeechToTextWidgetTest(QObject *parent)
    : QObject{parent}
{
    QStandardPaths::setTestModeEnabled(true);
}

void SieveEditorConfigureSpeechToTextWidgetTest::shouldHaveDefaultValues()
{
    const SieveEditorConfigureSpeechToTextWidget w;
    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mEnableSpeechToText = w.findChild<QCheckBox *>(u"mEnableSpeechToText"_s);
    QVERIFY(mEnableSpeechToText);
    QVERIFY(!mEnableSpeechToText->text().isEmpty());
    QVERIFY(!mEnableSpeechToText->isChecked());

    auto mSpeechToTextWidget = w.findChild<TextSpeechToText::SpeechToTextConfigureWidget *>(u"mSpeechToTextWidget"_s);
    QVERIFY(mSpeechToTextWidget);
}

#include "moc_sieveeditorconfigurespeechtotextwidgettest.cpp"
