/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "addnewscripttoolinternaljob.h"
#include "sieveeditor_debug.h"
#include "sieveeditormainwidget.h"
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
        for (const auto &resultTool : std::as_const(mToolArguments)) {
            if (resultTool.keyTool == "title"_L1) {
                mSieveEditorMainWidget->createNewScript(); // TODO add title
                // TODO
            } else {
                qCWarning(SIEVEEDITOR_LOG) << "Invalid json tools result:" << resultTool.keyTool;
            }
        }
        deleteLater();
        // TODO
        Q_EMIT finished({});
    } else {
        qCWarning(SIEVEEDITOR_LOG) << "mSieveEditorMainWidget is not settings. It's a bug";
        deleteLater();
    }
}

#include "moc_addnewscripttoolinternaljob.cpp"
