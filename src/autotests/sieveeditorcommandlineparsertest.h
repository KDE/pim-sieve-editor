/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#pragma once

#include <QObject>

class SieveEditorCommandLineParserTest : public QObject
{
    Q_OBJECT
public:
    explicit SieveEditorCommandLineParserTest(QObject *parent = nullptr);
    ~SieveEditorCommandLineParserTest() override = default;

private Q_SLOTS:
    void shouldVerifyOptionParserFromEnumValues();
};
