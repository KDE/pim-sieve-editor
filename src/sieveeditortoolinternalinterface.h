/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once
#include <TextAutoGenerateText/TextAutoGenerateTextToolInternalInterface>
class SieveEditorMainWidget;
class SieveEditorToolInternalInterface : public TextAutoGenerateText::TextAutoGenerateTextToolInternalInterface
{
    Q_OBJECT
public:
    explicit SieveEditorToolInternalInterface(SieveEditorMainWidget *sieveEditorMainWidget, QObject *parent = nullptr);
    ~SieveEditorToolInternalInterface() override;

    [[nodiscard]] TextAutoGenerateText::TextAutoGenerateTextToolInternalJob *callTool(const QByteArray &toolName) override;

private:
    SieveEditorMainWidget *const mSieveEditorMainWidget;
};
