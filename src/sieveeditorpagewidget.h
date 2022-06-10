/*
   SPDX-FileCopyrightText: 2014-2022 Laurent Montel <montel@kde.org>

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
    Q_REQUIRED_RESULT QUrl currentUrl() const;
    void setCurrentUrl(const QUrl &url);
    void setIsNewScript(bool isNewScript);
    void uploadScript(bool showInformation = true, bool forceSave = false);
    Q_REQUIRED_RESULT bool needToSaveScript();

    Q_REQUIRED_RESULT bool isModified() const;
    void goToLine();
    Q_REQUIRED_RESULT KSieveUi::SieveEditorWidget::EditorMode pageMode() const;

    void find();
    void replace();
    void redo();
    void undo();
    Q_REQUIRED_RESULT bool isUndoAvailable() const;
    Q_REQUIRED_RESULT bool isRedoAvailable() const;
    void paste();
    void cut();
    void copy();
    Q_REQUIRED_RESULT bool hasSelection() const;

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
    Q_REQUIRED_RESULT QString currentText() const;
    Q_REQUIRED_RESULT QString currentHelpTitle() const;
    Q_REQUIRED_RESULT QUrl currentHelpUrl() const;
    void openBookmarkUrl(const QUrl &url);
    void debugSieveScript();
    void zoomReset();
    void setWordWrap(bool state);
    Q_REQUIRED_RESULT bool isWordWrap() const;
    void print();
    void printPreview();
    bool printSupportEnabled() const;
    bool isTextEditor() const;
    void renameScriptName(const QString &newName);
    Q_REQUIRED_RESULT bool uploadScriptAndCloseTab(int index);
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

private:
    void setModified(bool b);
    QUrl mCurrentURL;
    KSieveUi::SieveEditorWidget *mSieveEditorWidget = nullptr;
    bool mWasActive = false;
    bool mIsNewScript = false;
};
