/*
   SPDX-FileCopyrightText: 2013-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "sieveeditorscriptmanagerwidget.h"
#include "sieveeditormanagesievewidget.h"

#include <QHBoxLayout>
#include <QTreeWidget>

SieveEditorScriptManagerWidget::SieveEditorScriptManagerWidget(QWidget *parent)
    : QWidget(parent)
    , mTreeView(new SieveEditorManageSieveWidget(this))
{
    auto hbox = new QHBoxLayout(this);
    hbox->setContentsMargins({});
    connect(mTreeView, &SieveEditorManageSieveWidget::newScript, this, &SieveEditorScriptManagerWidget::slotNewScript);
    connect(mTreeView, &SieveEditorManageSieveWidget::editScript, this, &SieveEditorScriptManagerWidget::slotEditScript);
    connect(mTreeView, &SieveEditorManageSieveWidget::updateButtons, this, &SieveEditorScriptManagerWidget::slotUpdateButtons);
    connect(mTreeView, &SieveEditorManageSieveWidget::scriptDeleted, this, &SieveEditorScriptManagerWidget::scriptDeleted);
    connect(mTreeView, &SieveEditorManageSieveWidget::serverSieveFound, this, &SieveEditorScriptManagerWidget::serverSieveFound);
    connect(mTreeView, &SieveEditorManageSieveWidget::scriptRenamed, this, &SieveEditorScriptManagerWidget::scriptRenamed);
    hbox->addWidget(mTreeView);
}

SieveEditorScriptManagerWidget::~SieveEditorScriptManagerWidget() = default;

void SieveEditorScriptManagerWidget::slotUpdateButtons(QTreeWidgetItem *item)
{
    Q_UNUSED(item)
    bool newScriptAction;
    bool editScriptAction;
    bool deleteScriptAction;
    bool desactivateScriptAction;
    mTreeView->enableDisableActions(newScriptAction, editScriptAction, deleteScriptAction, desactivateScriptAction);
    Q_EMIT updateButtons(newScriptAction, editScriptAction, deleteScriptAction, desactivateScriptAction);
}

void SieveEditorScriptManagerWidget::slotEditScript(const KSieveUi::ManageSieveWidget::ScriptInfo &info)
{
    Q_EMIT createScriptPage(info, false);
}

void SieveEditorScriptManagerWidget::slotNewScript(const KSieveUi::ManageSieveWidget::ScriptInfo &info)
{
    Q_EMIT createScriptPage(info, true);
}

void SieveEditorScriptManagerWidget::slotCreateNewScript()
{
    mTreeView->slotNewScript();
}

void SieveEditorScriptManagerWidget::slotDeleteScript()
{
    mTreeView->slotDeleteScript();
}

void SieveEditorScriptManagerWidget::slotRefreshList()
{
    updateServerList();
}

void SieveEditorScriptManagerWidget::updateServerList()
{
    mTreeView->slotRefresh();
}

void SieveEditorScriptManagerWidget::editScript()
{
    mTreeView->slotEditScript();
}

void SieveEditorScriptManagerWidget::desactivateScript()
{
    mTreeView->slotDeactivateScript();
}

void SieveEditorScriptManagerWidget::refreshList()
{
    mTreeView->slotRefresh();
}

#include "moc_sieveeditorscriptmanagerwidget.cpp"
