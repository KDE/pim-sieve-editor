/*
   Copyright (C) 2015-2020 Laurent Montel <montel@kde.org>

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
