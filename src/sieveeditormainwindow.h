/*
   SPDX-FileCopyrightText: 2013-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "config-pim-sieve-editor.h"
#include "libsieveeditor_export.h"
#include <KXmlGuiWindow>
class QAction;
class SieveEditorCentralWidget;
class QLabel;
class SieveEditorBookmarks;
class KActionMenu;
class KToggleFullScreenAction;
class KHamburgerMenu;
class KToggleAction;
#if defined(Q_OS_WIN) || defined(Q_OS_MACOS)
#if HAVE_TEXTUTILS_HAS_WHATSNEW_SUPPORT
namespace TextAddonsWidgets
{
class VerifyNewVersionWidget;
}
#endif
#endif
namespace PimCommon
{
class KActionMenuChangeCase;
#if defined(Q_OS_WIN) || defined(Q_OS_MACOS)
#if !HAVE_TEXTUTILS_HAS_WHATSNEW_SUPPORT
class VerifyNewVersionWidget;
#endif
#endif
}
class LIBSIEVEEDITOR_EXPORT SieveEditorMainWindow : public KXmlGuiWindow
{
    Q_OBJECT
public:
    explicit SieveEditorMainWindow(QWidget *parent = nullptr);
    ~SieveEditorMainWindow() override;

    [[nodiscard]] QString currentHelpTitle() const;
    [[nodiscard]] QUrl currentHelpUrl() const;

    void slotActivateRequested(const QStringList &arguments, const QString &workingDirectory);

protected:
    void closeEvent(QCloseEvent *e) override;

private:
    LIBSIEVEEDITOR_NO_EXPORT void slotConfigure();
    LIBSIEVEEDITOR_NO_EXPORT void slotAddServerSieve();
    LIBSIEVEEDITOR_NO_EXPORT void slotCreateNewScript();
    LIBSIEVEEDITOR_NO_EXPORT void slotDeleteScript();
    LIBSIEVEEDITOR_NO_EXPORT void slotUpdateButtons(bool newScriptAction, bool editScriptAction, bool deleteScriptAction, bool desactivateScriptAction);
    LIBSIEVEEDITOR_NO_EXPORT void slotEditScript();
    LIBSIEVEEDITOR_NO_EXPORT void slotDesactivateScript();
    LIBSIEVEEDITOR_NO_EXPORT void slotRefreshList();
    LIBSIEVEEDITOR_NO_EXPORT void slotUploadScript();
    LIBSIEVEEDITOR_NO_EXPORT void slotUpdateActions();
    LIBSIEVEEDITOR_NO_EXPORT void slotSystemNetworkOnlineStateChanged(bool state);
    LIBSIEVEEDITOR_NO_EXPORT void slotUndoAvailable(bool);
    LIBSIEVEEDITOR_NO_EXPORT void slotRedoAvailable(bool);
    LIBSIEVEEDITOR_NO_EXPORT void slotCopyAvailable(bool b);
    LIBSIEVEEDITOR_NO_EXPORT void slotOpenBookmarkUrl(const QUrl &url);
    LIBSIEVEEDITOR_NO_EXPORT void slotImportImapSettings();
    LIBSIEVEEDITOR_NO_EXPORT void initStatusBar();
    LIBSIEVEEDITOR_NO_EXPORT void readConfig();
    LIBSIEVEEDITOR_NO_EXPORT void setupActions();
    [[nodiscard]] LIBSIEVEEDITOR_NO_EXPORT QString currentText() const;
    LIBSIEVEEDITOR_NO_EXPORT void slotFullScreen(bool t);
    LIBSIEVEEDITOR_NO_EXPORT void writeConfig();
    LIBSIEVEEDITOR_NO_EXPORT void slotToggleMenubar(bool dontShowWarning);
    LIBSIEVEEDITOR_NO_EXPORT void updateHamburgerMenu();
    LIBSIEVEEDITOR_NO_EXPORT void slotWhatsNew();

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
    KToggleFullScreenAction *mShowFullScreenAction = nullptr;
    KHamburgerMenu *mHamburgerMenu = nullptr;
    KToggleAction *mShowMenuBarAction = nullptr;
#if defined(Q_OS_WIN) || defined(Q_OS_MACOS)
#if HAVE_TEXTUTILS_HAS_WHATSNEW_SUPPORT
    TextAddonsWidgets::VerifyNewVersionWidget *const mVerifyNewVersionWidget;
#else
    PimCommon::VerifyNewVersionWidget *const mVerifyNewVersionWidget;
#endif
#endif
    bool mNetworkIsDown = false;
};
