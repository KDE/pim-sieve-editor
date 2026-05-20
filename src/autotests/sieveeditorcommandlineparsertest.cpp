/*
 * SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "sieveeditorcommandlineparsertest.h"
#include "config-pim-sieve-editor.h"
#include "sieveeditorcommandlineparser.h"
#include <QTest>
QTEST_GUILESS_MAIN(SieveEditorCommandLineParserTest)
using namespace Qt::Literals::StringLiterals;
SieveEditorCommandLineParserTest::SieveEditorCommandLineParserTest(QObject *parent)
    : QObject{parent}
{
}

void SieveEditorCommandLineParserTest::shouldVerifyOptionParserFromEnumValues()
{
#if WITH_KUSERFEEDBACK
    QCOMPARE(SieveEditorCommandLineParser::optionParserFromEnum(SieveEditorCommandLineParser::OptionParser::FeedBack), u"feedback"_s);
#endif
    QCOMPARE(SieveEditorCommandLineParser::optionParserFromEnum(SieveEditorCommandLineParser::OptionParser::SelfTest), u"self-test"_s);
}

#include "moc_sieveeditorcommandlineparsertest.cpp"
