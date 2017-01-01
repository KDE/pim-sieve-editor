/*
   Copyright (C) 2014-2017 Montel Laurent <montel@kde.org>

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

#include "sieveeditorutiltest.h"
#include <qtest.h>
#include "../src/sieveeditorutil.h"

SieveEditorUtilTest::SieveEditorUtilTest(QObject *parent)
    : QObject(parent)
{

}

SieveEditorUtilTest::~SieveEditorUtilTest()
{

}

void SieveEditorUtilTest::shouldHaveDefaultValue()
{
    SieveEditorUtil::SieveServerConfig config;
    QVERIFY(config.sieveSettings.userName.isEmpty());
    QVERIFY(config.sieveSettings.password.isEmpty());
    QVERIFY(config.sieveSettings.serverName.isEmpty());
    QCOMPARE(config.sieveSettings.authenticationType, MailTransport::Transport::EnumAuthenticationType::PLAIN);
    QCOMPARE(config.sieveSettings.port, -1);
    QVERIFY(config.enabled);
    QVERIFY(!config.useImapCustomServer);
}

QTEST_APPLESS_MAIN(SieveEditorUtilTest)

