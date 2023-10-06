/*
   SPDX-FileCopyrightText: 2013-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
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
    ~SieveEditorMainWidget() override;

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
    [[nodiscard]] bool isUndoAvailable() const;
    [[nodiscard]] bool isRedoAvailable() const;
    [[nodiscard]] bool isWordWrap() const;

    [[nodiscard]] bool hasSelection() const;
    [[nodiscard]] QString currentHelpTitle() const;
    [[nodiscard]] QUrl currentHelpUrl() const;
    void openBookmarkUrl(const QUrl &url);
    [[nodiscard]] bool printSupportEnabled() const;
    [[nodiscard]] bool isTextEditor() const;
    [[nodiscard]] QString currentText() const;
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

protected:
    [[nodiscard]] bool event(QEvent *e) override;

private:
    void slotCreateScriptPage(const KSieveUi::ManageSieveWidget::ScriptInfo &info, bool isNewScript);
    void slotScriptDeleted(const QUrl &url);
    void slotScriptRenamed(const QUrl &oldUrl, const QUrl &newUrl);
    void slotScriptModified(bool modified, SieveEditorPageWidget *page);
    void slotGeneralPaletteChanged();
    void updatePaletteColor();
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
