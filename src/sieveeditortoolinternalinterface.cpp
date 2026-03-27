/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "sieveeditortoolinternalinterface.h"

SieveEditorToolInternalInterface::SieveEditorToolInternalInterface(QObject *parent)
    : TextAutoGenerateText::TextAutoGenerateTextToolInternalInterface{parent}
{
}

SieveEditorToolInternalInterface::~SieveEditorToolInternalInterface() = default;

TextAutoGenerateText::TextAutoGenerateTextToolInternalJob *SieveEditorToolInternalInterface::callTool(const QByteArray &toolName)
{
    Q_UNUSED(toolName);
    return {};
}
