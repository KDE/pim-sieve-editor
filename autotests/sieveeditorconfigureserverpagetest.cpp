/*
   SPDX-FileCopyrightText: 2015-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "sieveeditorconfigureserverpagetest.h"
#include "../src/sieveeditorconfigureserverpage.h"
#include <QPushButton>
#include <QTest>
#include <QSignalSpy>
#include <qtestmouse.h>
SieveEditorConfigureServerPageTest::SieveEditorConfigureServerPageTest(QObject *parent)
    : QObject(parent)
{
}

SieveEditorConfigureServerPageTest::~SieveEditorConfigureServerPageTest()
{
}

void SieveEditorConfigureServerPageTest::shouldHaveDefaultValue()
{
    SieveEditorConfigureServerPage w;
    QPushButton *button = w.findChild<QPushButton *>(QStringLiteral("configure_button"));
    QVERIFY(button);

    QPushButton *importSieveSettings = w.findChild<QPushButton *>(QStringLiteral("import_sieve_settings_button"));
    QVERIFY(importSieveSettings);
}

void SieveEditorConfigureServerPageTest::shouldEmitSignalWhenClickOnConfigureButton()
{
    SieveEditorConfigureServerPage w;
    QPushButton *button = w.findChild<QPushButton *>(QStringLiteral("configure_button"));
    QSignalSpy spy(&w, &SieveEditorConfigureServerPage::configureClicked);
    QTest::mouseClick(button, Qt::LeftButton);
    QCOMPARE(spy.count(), 1);
}

void SieveEditorConfigureServerPageTest::shouldEmitSignalWhenClickOnImportSieveButton()
{
    SieveEditorConfigureServerPage w;
    QPushButton *button = w.findChild<QPushButton *>(QStringLiteral("import_sieve_settings_button"));
    QSignalSpy spy(&w, &SieveEditorConfigureServerPage::importSieveSettings);
    QTest::mouseClick(button, Qt::LeftButton);
    QCOMPARE(spy.count(), 1);
}

QTEST_MAIN(SieveEditorConfigureServerPageTest)
