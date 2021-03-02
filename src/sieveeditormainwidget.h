/*
   Copyright (C) 2013-2021 Laurent Montel <montel@kde.org>

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

#pragma once

#include <KSieveUi/ManageSieveWidget>
#include <KSieveUi/SieveEditorWidget>
#include <QSplitter>
#include <QUrl>
class QTabWidget;
class QStackedWidget;
class SieveEditorTabWidget;
class SieveEditorScriptManagerWidget;
class SieveEditorPageWidget;
class SieveEditorEmptyTabWidgetLabel;
class KActionCollection;

class SieveEditorMainWidget : public QSplitter
{
    Q_OBJECT
public:
    explicit SieveEditorMainWidget(KActionCollection *ac, QWidget *parent = nullptr);
    ~SieveEditorMainWidget();

    void createNewScript();
    void deleteScript();
    void updateServerList();
    void editScript();
    void desactivateScript();
    void refreshList();
    void uploadScript();
    bool needToSaveScript();

    QTabWidget *tabWidget() const;

    KSieveUi::SieveEditorWidget::EditorMode pageMode() const;
    Q_REQUIRED_RESULT bool isUndoAvailable() const;
    Q_REQUIRED_RESULT bool isRedoAvailable() const;
    Q_REQUIRED_RESULT bool isWordWrap() const;

    Q_REQUIRED_RESULT bool hasSelection() const;
    Q_REQUIRED_RESULT QString currentHelpTitle() const;
    Q_REQUIRED_RESULT QUrl currentHelpUrl() const;
    void openBookmarkUrl(const QUrl &url);
    Q_REQUIRED_RESULT bool printSupportEnabled() const;
    Q_REQUIRED_RESULT bool isTextEditor() const;
    Q_REQUIRED_RESULT QString currentText() const;
    void forceCloseTab(int index);
public Q_SLOTS:
    void slotDebugSieveScript();
    void slotGoToLine();

    void slotReplace();
    void slotFind();
    void slotCopy();
    void slotPaste();
    void slotCut();
    void slotSaveAs();
    void slotCheckSpelling();
    void slotShareScript();
    void slotImport();
    void slotCreateRulesGraphically();
    void slotCheckSyntax();
    void slotComment();
    void slotUncomment();
    void slotUpperCase();
    void slotSentenceCase();
    void slotLowerCase();
    void slotReverseCase();
    void slotZoomIn();
    void slotZoomOut();
    void slotZoomReset();
    void slotWordWrap(bool);
    void slotPrintPreview();
    void slotPrint();
    void slotUndo();
    void slotRedo();
    void slotSelectAll();

Q_SIGNALS:
    void updateButtons(bool newScriptAction, bool editScriptAction, bool deleteScriptAction, bool desactivateScriptAction);
    void updateScriptList();
    void modeEditorChanged(KSieveUi::SieveEditorWidget::EditorMode);
    void serverSieveFound(bool);
    void undoAvailable(bool);
    void redoAvailable(bool);
    void copyAvailable(bool);
    void sieveEditorTabCurrentChanged();

private:
    void slotCreateScriptPage(const KSieveUi::ManageSieveWidget::ScriptInfo &info, bool isNewScript);
    void slotScriptDeleted(const QUrl &url);
    void slotScriptRenamed(const QUrl &oldUrl, const QUrl &newUrl);
    void slotScriptModified(bool modified, SieveEditorPageWidget *page);
    void slotGeneralPaletteChanged();
    void slotTabCloseRequested(int index);
    void slotTabRemoveAllExclude(int index);
    void slotTabCloseAllRequested();

private:
    void updateStackedWidget();
    QWidget *hasExistingPage(const QUrl &url);
    QColor mModifiedScriptColor;
    QColor mScriptColor;
    SieveEditorTabWidget *mTabWidget = nullptr;
    SieveEditorScriptManagerWidget *mScriptManagerWidget = nullptr;
    QStackedWidget *mStackedWidget = nullptr;
    SieveEditorEmptyTabWidgetLabel *mEditorEmptyLabel = nullptr;
};

