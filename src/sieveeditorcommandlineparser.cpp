/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "sieveeditorcommandlineparser.h"
#include <KLocalizedString>
#include <QCommandLineParser>

using namespace Qt::Literals::StringLiterals;
SieveEditorCommandLineParser::SieveEditorCommandLineParser(QCommandLineParser *parser)
{
    initializeCommandLine(parser);
}

SieveEditorCommandLineParser::~SieveEditorCommandLineParser() = default;

void SieveEditorCommandLineParser::initializeCommandLine(QCommandLineParser *parser)
{
#if WITH_KUSERFEEDBACK
    parser->addOption(QCommandLineOption(optionParserFromEnum(OptionParser::FeedBack), i18nc("@info:shell", "Lists the available options for user feedback")));
#endif
    QCommandLineOption selfTestOption(optionParserFromEnum(OptionParser::SelfTest), QStringLiteral("internal, for automated testing"));
    selfTestOption.setFlags(QCommandLineOption::HiddenFromHelp);
    parser->addOption(selfTestOption);
}

QString SieveEditorCommandLineParser::optionParserFromEnum(OptionParser e)
{
    switch (e) {
#if WITH_KUSERFEEDBACK
    case OptionParser::FeedBack:
        return u"feedback"_s;
#endif
    case OptionParser::SelfTest:
        return u"self-test"_s;
    }
    return {};
}
