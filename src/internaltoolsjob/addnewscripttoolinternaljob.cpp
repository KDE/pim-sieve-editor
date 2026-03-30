/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "addnewscripttoolinternaljob.h"
using namespace Qt::Literals::StringLiterals;
AddNewScriptToolInternalJob::AddNewScriptToolInternalJob(SieveEditorMainWidget *sieveEditorMainWidget, QObject *parent)
    : TextAutoGenerateText::TextAutoGenerateTextToolInternalJob{parent}
    , mSieveEditorMainWidget(sieveEditorMainWidget)
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

void AddNewScriptToolInternalJob::start()
{
    if (mSieveEditorMainWidget) {
        const QStringList lst = requiredArguments();
        for (const auto &arg : lst) {
            for (const auto &resultTool : std::as_const(mToolArguments)) {
                if (resultTool.keyTool == arg) {
                    // TODO
                }
            }
        }
        // TODO
    } else {
        deleteLater();
    }
    // TODO
}

#include "moc_addnewscripttoolinternaljob.cpp"
