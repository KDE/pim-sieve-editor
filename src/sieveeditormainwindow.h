/*
   SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "libsieveeditor_export.h"
#include <KXmlGuiWindow>
class QAction;
class SieveEditorCentralWidget;
class QLabel;
class SieveEditorBookmarks;
class KActionMenu;
namespace PimCommon
{
class KActionMenuChangeCase;
}
class LIBSIEVEEDITOR_EXPORT SieveEditorMainWindow : public KXmlGuiWindow
{
    Q_OBJECT
public:
    explicit SieveEditorMainWindow(QWidget *parent = nullptr);
    ~SieveEditorMainWindow() override;

    Q_REQUIRED_RESULT QString currentHelpTitle() const;
    Q_REQUIRED_RESULT QUrl currentHelpUrl() const;

protected:
    void closeEvent(QCloseEvent *e) override;

private:
    void slotConfigure();
    void slotAddServerSieve();
    void slotCreateNewScript();
    void slotDeleteScript();
    void slotUpdateButtons(bool newScriptAction, bool editScriptAction, bool deleteScriptAction, bool desactivateScriptAction);
    void slotEditScript();
    void slotDesactivateScript();
    void slotRefreshList();
    void slotUploadScript();
    void slotUpdateActions();
    void slotSystemNetworkOnlineStateChanged(bool state);
    void slotUndoAvailable(bool);
    void slotRedoAvailable(bool);
    void slotCopyAvailable(bool b);
    void slotOpenBookmarkUrl(const QUrl &url);
    void slotImportImapSettings();

private:
    void initStatusBar();
    void readConfig();
    void setupActions();
    QString currentText() const;

    SieveEditorCentralWidget *mMainWidget = nullptr;
    QAction *mDeleteScript = nullptr;
    QAction *mNewScript = nullptr;
    QAction *mEditScript = nullptr;
    QAction *mDesactivateScript = nullptr;
    QAction *mRefreshList = nullptr;
    QAction *mUploadScript = nullptr;
    QAction *mGoToLine = nullptr;
    QAction *mFindAction = nullptr;
    QAction *mReplaceAction = nullptr;
    QAction *mUndoAction = nullptr;
    QAction *mRedoAction = nullptr;
    QAction *mCopyAction = nullptr;
    QAction *mPasteAction = nullptr;
    QAction *mCutAction = nullptr;
    QAction *mSelectAllAction = nullptr;
    QAction *mSaveAsAction = nullptr;
    QAction *mImportAction = nullptr;
    QAction *mShareAction = nullptr;
    QAction *mSpellCheckAction = nullptr;
    QAction *mCheckSyntaxAction = nullptr;
    QAction *mCreateRulesGraphicallyAction = nullptr;
    QAction *mCommentAction = nullptr;
    QAction *mUncommentAction = nullptr;
    QAction *mZoomInAction = nullptr;
    QAction *mZoomOutAction = nullptr;
    QAction *mZoomResetAction = nullptr;
    QAction *mDebugSieveScriptAction = nullptr;
    QAction *mWrapTextAction = nullptr;
    QAction *mPrintAction = nullptr;
    QAction *mPrintPreviewAction = nullptr;
    PimCommon::KActionMenuChangeCase *mMenuChangeCaseAction = nullptr;

    QLabel *mStatusBarInfo = nullptr;
    SieveEditorBookmarks *mSieveEditorBookmarks = nullptr;
    KActionMenu *mBookmarkMenu = nullptr;
    bool mNetworkIsDown = false;
};

