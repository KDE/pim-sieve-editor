/*
   Copyright (C) 2013-2018 Montel Laurent <montel@kde.org>

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

#ifndef SIEVEEDITORSCRIPTMANAGERWIDGET_H
#define SIEVEEDITORSCRIPTMANAGERWIDGET_H

#include <QWidget>
#include <QUrl>
#include <KSieveUi/ManageSieveWidget>

class SieveEditorManageSieveWidget;
class QTreeWidgetItem;
namespace KSieveUi {
class SieveImapAccountSettings;
}
class SieveEditorScriptManagerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SieveEditorScriptManagerWidget(QWidget *parent = nullptr);
    ~SieveEditorScriptManagerWidget();

    void updateServerList();
    void editScript();
    void desactivateScript();
    void refreshList();

Q_SIGNALS:
    void createScriptPage(const KSieveUi::ManageSieveWidget::ScriptInfo &info, bool isNewScript);
    void updateButtons(bool newScriptAction, bool editScriptAction, bool deleteScriptAction, bool desactivateScriptAction);
    void scriptDeleted(const QUrl &url);
    void serverSieveFound(bool hasServerSieve);
    void scriptRenamed(const QUrl &oldUrl, const QUrl &newUrl);

public Q_SLOTS:
    void slotCreateNewScript();
    void slotDeleteScript();
    void slotRefreshList();

private Q_SLOTS:
    void slotNewScript(const KSieveUi::ManageSieveWidget::ScriptInfo &info);
    void slotEditScript(const KSieveUi::ManageSieveWidget::ScriptInfo &info);
    void slotUpdateButtons(QTreeWidgetItem *item);

private:
    SieveEditorManageSieveWidget *mTreeView = nullptr;
};

#endif // SIEVEEDITORSCRIPTMANAGERWIDGET_H
