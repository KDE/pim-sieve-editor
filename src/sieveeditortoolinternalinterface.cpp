/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "sieveeditortoolinternalinterface.h"
#include "internaltoolsjob/addnewscripttoolinternaljob.h"
using namespace Qt::Literals::StringLiterals;
SieveEditorToolInternalInterface::SieveEditorToolInternalInterface(QObject *parent)
    : TextAutoGenerateText::TextAutoGenerateTextToolInternalInterface{parent}
{
}

SieveEditorToolInternalInterface::~SieveEditorToolInternalInterface() = default;

TextAutoGenerateText::TextAutoGenerateTextToolInternalJob *SieveEditorToolInternalInterface::callTool(const QByteArray &toolName)
{
    TextAutoGenerateText::TextAutoGenerateTextToolInternalJob *job = nullptr;
    if (toolName == AddNewScriptToolInternalJob::toolId()) {
        job = new AddNewScriptToolInternalJob(this);
    }
    // TODO
    return job;
}

#include "moc_sieveeditortoolinternalinterface.cpp"
