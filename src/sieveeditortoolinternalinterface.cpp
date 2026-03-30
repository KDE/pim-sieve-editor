/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "sieveeditortoolinternalinterface.h"
#include "internaltoolsjob/addnewscripttoolinternaljob.h"
#include "sieveeditor_debug.h"
using namespace Qt::Literals::StringLiterals;
SieveEditorToolInternalInterface::SieveEditorToolInternalInterface(SieveEditorMainWidget *sieveEditorMainWidget, QObject *parent)
    : TextAutoGenerateText::TextAutoGenerateTextToolInternalInterface{parent}
    , mSieveEditorMainWidget(sieveEditorMainWidget)
{
}

SieveEditorToolInternalInterface::~SieveEditorToolInternalInterface() = default;

TextAutoGenerateText::TextAutoGenerateTextToolInternalJob *SieveEditorToolInternalInterface::callTool(const QByteArray &toolName)
{
    TextAutoGenerateText::TextAutoGenerateTextToolInternalJob *job = nullptr;
    if (toolName == AddNewScriptToolInternalJob::toolId()) {
        job = new AddNewScriptToolInternalJob(mSieveEditorMainWidget, this);
    }
    qCWarning(SIEVEEDITOR_LOG) << "Impossible to find tool " << toolName;
    // TODO
    return job;
}

#include "moc_sieveeditortoolinternalinterface.cpp"
