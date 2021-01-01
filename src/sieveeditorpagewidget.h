/*
   Copyright (C) 2014-2021 Laurent Montel <montel@kde.org>

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

#ifndef SIEVEEDITORPAGEWIDGET_H
#define SIEVEEDITORPAGEWIDGET_H
#include <QWidget>
#include <QUrl>
#include <KSieveUi/SieveEditorWidget>
#include <KSieveUi/ManageSieveWidget>
namespace KManageSieve {
class SieveJob;
}
namespace KSieveUi {
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
    void shareScript();
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
    void wordWrap(bool state);
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

#endif // SIEVEEDITORPAGEWIDGET_H
