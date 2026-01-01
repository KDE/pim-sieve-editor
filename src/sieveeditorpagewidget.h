/*
   SPDX-FileCopyrightText: 2014-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once
#include <KSieveUi/ManageSieveWidget>
#include <KSieveUi/SieveEditorWidget>
#include <QUrl>
#include <QWidget>
namespace KManageSieve
{
class SieveJob;
}
namespace KSieveUi
{
class SieveEditorWidget;
}

class SieveEditorPageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SieveEditorPageWidget(QWidget *parent = nullptr);
    ~SieveEditorPageWidget() override;

    void loadScript(const KSieveUi::ManageSieveWidget::ScriptInfo &info);
    [[nodiscard]] QUrl currentUrl() const;
    void setCurrentUrl(const QUrl &url);
    void setIsNewScript(bool isNewScript);
    void uploadScript(bool showInformation = true, bool forceSave = false);
    [[nodiscard]] bool needToSaveScript();

    [[nodiscard]] bool isModified() const;
    void goToLine();
    [[nodiscard]] KSieveUi::SieveEditorWidget::EditorMode pageMode() const;

    void find();
    void replace();
    void redo();
    void undo();
    [[nodiscard]] bool isUndoAvailable() const;
    [[nodiscard]] bool isRedoAvailable() const;
    void paste();
    void cut();
    void copy();
    [[nodiscard]] bool hasSelection() const;

    void selectAll();
    void saveAs();
    void checkSpelling();
    void import();
    void createRulesGraphically();
    void checkSyntax();
    void comment();
    void uncomment();
    void lowerCase();
    void upperCase();
    void sentenceCase();
    void reverseCase();
    void zoomIn();
    void zoomOut();
    [[nodiscard]] QString currentText() const;
    [[nodiscard]] QString currentHelpTitle() const;
    [[nodiscard]] QUrl currentHelpUrl() const;
    void openBookmarkUrl(const QUrl &url);
    void debugSieveScript();
    void zoomReset();
    void setWordWrap(bool state);
    [[nodiscard]] bool isWordWrap() const;
    void print();
    void printPreview();
    bool printSupportEnabled() const;
    bool isTextEditor() const;
    void renameScriptName(const QString &newName);
    [[nodiscard]] bool uploadScriptAndCloseTab(int index);
Q_SIGNALS:
    void refreshList();
    void scriptModified(bool, SieveEditorPageWidget *);
    void modeEditorChanged(KSieveUi::SieveEditorWidget::EditorMode);
    void undoAvailable(bool);
    void redoAvailable(bool);
    void copyAvailable(bool);
    void sieveEditorTabCurrentChanged();
    void requestCloseTab(int index);

private:
    void slotGetResult(KManageSieve::SieveJob *, bool success, const QString &script, bool isActive);
    void slotCheckSyntaxClicked();
    void slotPutResult(KManageSieve::SieveJob *, bool success);
    void slotValueChanged(bool b);
    void slotCheckScriptJobFinished(const QString &errorMsg, bool success);
    void setModified(bool b);
    QUrl mCurrentURL;
    KSieveUi::SieveEditorWidget *const mSieveEditorWidget;
    bool mWasActive = false;
    bool mIsNewScript = false;
};
