/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "addnewscripttoolinternaljob.h"
using namespace Qt::Literals::StringLiterals;
AddNewScriptToolInternalJob::AddNewScriptToolInternalJob(QObject *parent)
    : TextAutoGenerateText::TextAutoGenerateTextToolInternalJob{parent}
{
}

AddNewScriptToolInternalJob::~AddNewScriptToolInternalJob() = default;

QByteArray AddNewScriptToolInternalJob::toolName() const
{
    return toolId();
}

QByteArray AddNewScriptToolInternalJob::toolId()
{
    return "create_new_script"_ba;
}
