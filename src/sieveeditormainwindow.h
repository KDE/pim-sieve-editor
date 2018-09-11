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

#ifndef SIEVEEDITORMAINWINDOW_H
#define SIEVEEDITORMAINWINDOW_H

#include <KXmlGuiWindow>
#include "config-sieveeditor.h"
#include "libsieveeditor_export.h"

class QAction;
class SieveEditorCentralWidget;
class QLabel;
class SieveEditorBookmarks;
class KActionMenu;
namespace PimCommon {
class KActionMenuChangeCase;
}
namespace Purpose {
class Menu;
}
class LIBSIEVEEDITOR_EXPORT SieveEditorMainWindow : public KXmlGuiWindow
{
    Q_OBJECT
public:
    explicit SieveEditorMainWindow(QWidget *parent = nullptr);
    ~SieveEditorMainWindow() override;

    QString currentHelpTitle() const;
    QUrl currentHelpUrl() const;

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
    void slotShareActionFinished(const QJsonObject &output, int error, const QString &message);

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
#ifdef KF5_USE_PURPOSE
    Purpose::Menu *mShareMenu = nullptr;
#endif
    bool mNetworkIsDown = false;
};

#endif // SIEVEEDITORMAINWINDOW_H
