/*
   Copyright (C) 2013-2020 Laurent Montel <montel@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "sieveeditorscriptmanagerwidget.h"
#include "sieveeditormanagesievewidget.h"

#include <QHBoxLayout>
#include <QTreeWidget>

SieveEditorScriptManagerWidget::SieveEditorScriptManagerWidget(QWidget *parent)
    : QWidget(parent)
{
    auto *hbox = new QHBoxLayout(this);
    mTreeView = new SieveEditorManageSieveWidget(this);
    connect(mTreeView, &SieveEditorManageSieveWidget::newScript, this, &SieveEditorScriptManagerWidget::slotNewScript);
    connect(mTreeView, &SieveEditorManageSieveWidget::editScript, this, &SieveEditorScriptManagerWidget::slotEditScript);
    connect(mTreeView, &SieveEditorManageSieveWidget::updateButtons, this, &SieveEditorScriptManagerWidget::slotUpdateButtons);
    connect(mTreeView, &SieveEditorManageSieveWidget::scriptDeleted, this, &SieveEditorScriptManagerWidget::scriptDeleted);
    connect(mTreeView, &SieveEditorManageSieveWidget::serverSieveFound, this, &SieveEditorScriptManagerWidget::serverSieveFound);
    connect(mTreeView, &SieveEditorManageSieveWidget::scriptRenamed, this, &SieveEditorScriptManagerWidget::scriptRenamed);
    hbox->addWidget(mTreeView);
}

SieveEditorScriptManagerWidget::~SieveEditorScriptManagerWidget()
{
}

void SieveEditorScriptManagerWidget::slotUpdateButtons(QTreeWidgetItem *item)
{
    Q_UNUSED(item);
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
