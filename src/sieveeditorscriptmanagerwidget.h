/*
   SPDX-FileCopyrightText: 2013-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef SIEVEEDITORSCRIPTMANAGERWIDGET_H
#define SIEVEEDITORSCRIPTMANAGERWIDGET_H

#include <QWidget>
#include <QUrl>
#include <KSieveUi/ManageSieveWidget>

class SieveEditorManageSieveWidget;
class QTreeWidgetItem;
namespace KSieveUi {
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

private:
    void slotNewScript(const KSieveUi::ManageSieveWidget::ScriptInfo &info);
    void slotEditScript(const KSieveUi::ManageSieveWidget::ScriptInfo &info);
    void slotUpdateButtons(QTreeWidgetItem *item);

private:
    SieveEditorManageSieveWidget *mTreeView = nullptr;
};

#endif // SIEVEEDITORSCRIPTMANAGERWIDGET_H
