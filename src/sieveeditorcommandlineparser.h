/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#pragma once
#include "config-pim-sieve-editor.h"
#include "libsieveeditor_export.h"
#include <QString>
class QCommandLineParser;
class LIBSIEVEEDITOR_EXPORT SieveEditorCommandLineParser
{
public:
    enum class OptionParser : uint8_t {
#if WITH_KUSERFEEDBACK
        FeedBack,
#endif
        SelfTest,
    };
    explicit SieveEditorCommandLineParser(QCommandLineParser *parser);
    ~SieveEditorCommandLineParser();

    [[nodiscard]] static QString optionParserFromEnum(OptionParser e);

private:
    LIBSIEVEEDITOR_NO_EXPORT void initializeCommandLine(QCommandLineParser *parser);
};
