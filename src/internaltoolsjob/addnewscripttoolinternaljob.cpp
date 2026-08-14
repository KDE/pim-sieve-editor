/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "addnewscripttoolinternaljob.h"
#include "sieveeditor_debug.h"
#include "sieveeditormainwidget.h"
#include <KLocalizedString>
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
    if (!canStart()) {
        qCWarning(SIEVEEDITOR_LOG) << "Impossible to start AddNewScriptToolInternalJob";
        emitFinished(i18n("Impossible to create a new sieve script: invalid arguments."));
        return;
    }
    if (!mSieveEditorMainWidget) {
        qCWarning(SIEVEEDITOR_LOG) << "mSieveEditorMainWidget is not set. It's a bug";
        emitFinished(i18n("Impossible to create a new sieve script."));
        return;
    }
    QString title;
    for (const auto &resultTool : std::as_const(mToolArguments)) {
        if (resultTool.keyTool == "title"_L1) {
            title = resultTool.value;
            mSieveEditorMainWidget->addNewScript(title);
        } else {
            qCWarning(SIEVEEDITOR_LOG) << "Invalid json tools result:" << resultTool.keyTool;
        }
    }
    emitFinished(i18n("Create New Sieve Script named \"%1\"", title));
}

void AddNewScriptToolInternalJob::emitFinished(const QString &content)
{
    const TextAutoGenerateText::TextAutoGenerateTextToolInternalJob::TextToolPluginInfo info{
        .content = content,
        .messageUuid = mMessageUuid,
        .chatId = mChatId,
        .toolIdentifier = mToolIdentifier,
        .attachementInfoList = {},
    };
    Q_EMIT finished(info);
    deleteLater();
}

#include "moc_addnewscripttoolinternaljob.cpp"
