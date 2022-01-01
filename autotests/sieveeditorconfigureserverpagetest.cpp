/*
   SPDX-FileCopyrightText: 2015-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "sieveeditorconfigureserverpagetest.h"
#include "../src/sieveeditorconfigureserverpage.h"
#include <QPushButton>
#include <QSignalSpy>
#include <QTest>
#include <qtestmouse.h>
SieveEditorConfigureServerPageTest::SieveEditorConfigureServerPageTest(QObject *parent)
    : QObject(parent)
{
}

SieveEditorConfigureServerPageTest::~SieveEditorConfigureServerPageTest() = default;

void SieveEditorConfigureServerPageTest::shouldHaveDefaultValue()
{
    SieveEditorConfigureServerPage w;
    auto button = w.findChild<QPushButton *>(QStringLiteral("configure_button"));
    QVERIFY(button);

    auto importSieveSettings = w.findChild<QPushButton *>(QStringLiteral("import_sieve_settings_button"));
    QVERIFY(importSieveSettings);
}

void SieveEditorConfigureServerPageTest::shouldEmitSignalWhenClickOnConfigureButton()
{
    SieveEditorConfigureServerPage w;
    auto button = w.findChild<QPushButton *>(QStringLiteral("configure_button"));
    QSignalSpy spy(&w, &SieveEditorConfigureServerPage::configureClicked);
    QTest::mouseClick(button, Qt::LeftButton);
    QCOMPARE(spy.count(), 1);
}

void SieveEditorConfigureServerPageTest::shouldEmitSignalWhenClickOnImportSieveButton()
{
    SieveEditorConfigureServerPage w;
    auto button = w.findChild<QPushButton *>(QStringLiteral("import_sieve_settings_button"));
    QSignalSpy spy(&w, &SieveEditorConfigureServerPage::importSieveSettings);
    QTest::mouseClick(button, Qt::LeftButton);
    QCOMPARE(spy.count(), 1);
}

QTEST_MAIN(SieveEditorConfigureServerPageTest)
