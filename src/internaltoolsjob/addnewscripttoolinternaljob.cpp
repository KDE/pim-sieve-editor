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
    if (mSieveEditorMainWidget) {
        QString title;
        for (const auto &resultTool : std::as_const(mToolArguments)) {
            if (resultTool.keyTool == "title"_L1) {
                title = resultTool.value;
                mSieveEditorMainWidget->createNewScript(); // TODO add title
            } else {
                qCWarning(SIEVEEDITOR_LOG) << "Invalid json tools result:" << resultTool.keyTool;
            }
        }
        const TextAutoGenerateText::TextAutoGenerateTextToolInternalJob::TextToolPluginInfo info{
            .content = i18n("Create New Sieve Script named %1", title),
            .messageUuid = mMessageUuid,
            .chatId = mChatId,
            .toolIdentifier = mToolIdentifier,
            .attachementInfoList = {},
        };
        Q_EMIT finished(info);
    } else {
        qCWarning(SIEVEEDITOR_LOG) << "mSieveEditorMainWidget is not settings. It's a bug";
    }
    deleteLater();
}

#include "moc_addnewscripttoolinternaljob.cpp"
