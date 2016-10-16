/*
   Copyright (C) 2013-2016 Montel Laurent <montel@kde.org>

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

#include "sieveeditormainwindow.h"
#include "sieveeditormainwidget.h"
#include "sieveeditorconfiguredialog.h"
#include "serversievesettingsdialog.h"
#include "sieveserversettings.h"
#include "sieveeditorcentralwidget.h"
#include "sieveeditorglobalconfig.h"
#include "sieveeditorbookmarks.h"
#include "PimCommon/KActionMenuChangeCase"

#include <KStandardGuiItem>
#include <KSharedConfig>
#include <KIconEngine>
#include <KIconLoader>

#include <KLocalizedString>
#include <KConfigGroup>
#include <KStandardAction>
#include <KActionCollection>
#include <QAction>
#include <QStatusBar>
#include <QIcon>

#include <QPointer>
#include <QLabel>
#include <QCloseEvent>
#include <QNetworkConfigurationManager>

SieveEditorMainWindow::SieveEditorMainWindow()
    : KXmlGuiWindow()
{
    mMainWidget = new SieveEditorCentralWidget(this);
    connect(mMainWidget, &SieveEditorCentralWidget::configureClicked, this, &SieveEditorMainWindow::slotConfigure);
    connect(mMainWidget->sieveEditorMainWidget(), &SieveEditorMainWidget::updateButtons, this, &SieveEditorMainWindow::slotUpdateButtons);
    setCentralWidget(mMainWidget);
    setupActions();
    setupGUI();
    readConfig();
    initStatusBar();
    mNetworkConfigurationManager = new QNetworkConfigurationManager();
    connect(mNetworkConfigurationManager, &QNetworkConfigurationManager::onlineStateChanged, this, &SieveEditorMainWindow::slotSystemNetworkOnlineStateChanged);

    connect(mMainWidget->sieveEditorMainWidget()->tabWidget(), &QTabWidget::currentChanged, this, &SieveEditorMainWindow::slotUpdateActions);
    connect(mMainWidget->sieveEditorMainWidget(), &SieveEditorMainWidget::modeEditorChanged, this, &SieveEditorMainWindow::slotUpdateActions);
    slotSystemNetworkOnlineStateChanged(mNetworkConfigurationManager->isOnline());
    connect(mMainWidget->sieveEditorMainWidget(), &SieveEditorMainWidget::undoAvailable, this, &SieveEditorMainWindow::slotUndoAvailable);
    connect(mMainWidget->sieveEditorMainWidget(), &SieveEditorMainWidget::redoAvailable, this, &SieveEditorMainWindow::slotRedoAvailable);
    connect(mMainWidget->sieveEditorMainWidget(), &SieveEditorMainWidget::copyAvailable, this, &SieveEditorMainWindow::slotCopyAvailable);
    connect(mMainWidget->sieveEditorMainWidget(), &SieveEditorMainWidget::sieveEditorTabCurrentChanged, this, &SieveEditorMainWindow::slotUpdateActions);
    mMainWidget->sieveEditorMainWidget()->refreshList();
}

SieveEditorMainWindow::~SieveEditorMainWindow()
{
    KSharedConfig::Ptr config = KSharedConfig::openConfig();
    KConfigGroup group = config->group(QStringLiteral("SieveEditorMainWindow"));
    group.writeEntry("Size", size());
    if (SieveEditorGlobalConfig::self()->closeWallet()) {
        SieveServerSettings::self()->closeWallet();
    }
}

void SieveEditorMainWindow::initStatusBar()
{
    mStatusBarInfo = new QLabel;
    statusBar()->insertWidget(0, mStatusBarInfo, 4);
}

void SieveEditorMainWindow::slotSystemNetworkOnlineStateChanged(bool state)
{
    if (state) {
        mStatusBarInfo->setText(i18n("Network is Up."));
    } else {
        mStatusBarInfo->setText(i18n("Network is Down."));
    }
    mMainWidget->sieveEditorMainWidget()->setEnabled(state);
    slotUpdateActions();
}

void SieveEditorMainWindow::slotUpdateButtons(bool newScriptAction, bool editScriptAction, bool deleteScriptAction, bool desactivateScriptAction)
{
    mDeleteScript->setEnabled(deleteScriptAction);
    mNewScript->setEnabled(newScriptAction);
    mEditScript->setEnabled(editScriptAction);
    mDesactivateScript->setEnabled(desactivateScriptAction);
}

void SieveEditorMainWindow::readConfig()
{
    KSharedConfig::Ptr config = KSharedConfig::openConfig();
    KConfigGroup group = KConfigGroup(config, "SieveEditorMainWindow");
    const QSize sizeDialog = group.readEntry("Size", QSize(800, 600));
    if (sizeDialog.isValid()) {
        resize(sizeDialog);
    }
}

void SieveEditorMainWindow::setupActions()
{
    KActionCollection *ac = actionCollection();

    KStandardAction::quit(this, &SieveEditorMainWindow::close, ac);
    KStandardAction::preferences(this, &SieveEditorMainWindow::slotConfigure, ac);

    mUploadScript = KStandardAction::save(this, &SieveEditorMainWindow::slotUploadScript, ac);
    mUploadScript->setText(i18n("Upload"));
    mUploadScript->setEnabled(false);

    QAction *act = new QAction(i18n("Add Sieve Server..."), this);
    connect(act, &QAction::triggered, this, &SieveEditorMainWindow::slotAddServerSieve);
    ac->addAction(QStringLiteral("add_server_sieve"), act);

    mDeleteScript = new QAction(i18n("Delete Script"), this);
    connect(mDeleteScript, &QAction::triggered, this, &SieveEditorMainWindow::slotDeleteScript);
    ac->addAction(QStringLiteral("delete_script"), mDeleteScript);
    ac->setDefaultShortcut(mDeleteScript, QKeySequence(Qt::Key_Delete));
    mDeleteScript->setEnabled(false);

    mNewScript = new QAction(i18n("Create New Script..."), this);
    connect(mNewScript, &QAction::triggered, this, &SieveEditorMainWindow::slotCreateNewScript);
    ac->addAction(QStringLiteral("create_new_script"), mNewScript);
    ac->setDefaultShortcut(mNewScript, QKeySequence(Qt::CTRL + Qt::Key_N));
    mNewScript->setEnabled(false);

    mEditScript =  new QAction(i18n("Edit Script..."), this);
    connect(mEditScript, &QAction::triggered, this, &SieveEditorMainWindow::slotEditScript);
    ac->addAction(QStringLiteral("edit_script"), mEditScript);
    mEditScript->setEnabled(false);

    mDesactivateScript = new QAction(i18n("Deactivate Script"), this);
    connect(mDesactivateScript, &QAction::triggered, this, &SieveEditorMainWindow::slotDesactivateScript);
    ac->addAction(QStringLiteral("desactivate_script"), mDesactivateScript);
    mDesactivateScript->setEnabled(false);

    mRefreshList = new QAction(i18n("Refresh List"), this);
    connect(mRefreshList, &QAction::triggered, this, &SieveEditorMainWindow::slotRefreshList);
    ac->addAction(QStringLiteral("refresh_list"), mRefreshList);
    mRefreshList->setIcon(QIcon::fromTheme(QStringLiteral("view-refresh")));
    ac->setDefaultShortcut(mRefreshList, QKeySequence(Qt::Key_F5));

    mGoToLine = ac->addAction(QStringLiteral("gotoline"), mMainWidget->sieveEditorMainWidget(), SLOT(slotGoToLine()));
    mGoToLine->setText(i18n("Go to Line..."));
    mGoToLine->setIcon(QIcon::fromTheme(QStringLiteral("go-jump")));
    ac->setDefaultShortcut(mGoToLine, QKeySequence(Qt::CTRL + Qt::Key_G));
    mGoToLine->setEnabled(false);

    mFindAction = KStandardAction::find(mMainWidget->sieveEditorMainWidget(), &SieveEditorMainWidget::slotFind, ac);
    mReplaceAction = KStandardAction::replace(mMainWidget->sieveEditorMainWidget(), &SieveEditorMainWidget::slotReplace, ac);
    mUndoAction = KStandardAction::undo(mMainWidget->sieveEditorMainWidget(), &SieveEditorMainWidget::slotUndo, ac);
    mRedoAction = KStandardAction::redo(mMainWidget->sieveEditorMainWidget(), &SieveEditorMainWidget::slotRedo, ac);
    mCopyAction = KStandardAction::copy(mMainWidget->sieveEditorMainWidget(), &SieveEditorMainWidget::slotCopy, ac);
    mPasteAction = KStandardAction::paste(mMainWidget->sieveEditorMainWidget(), &SieveEditorMainWidget::slotPaste, ac);
    mCutAction = KStandardAction::cut(mMainWidget->sieveEditorMainWidget(), &SieveEditorMainWidget::slotCut, ac);
    mSelectAllAction = KStandardAction::selectAll(mMainWidget->sieveEditorMainWidget(), &SieveEditorMainWidget::slotSelectAll, ac);
    mSaveAsAction = KStandardAction::saveAs(mMainWidget->sieveEditorMainWidget(), &SieveEditorMainWidget::slotSaveAs, ac);

    mImportAction = new QAction(i18n("Import..."), this);
    connect(mImportAction, &QAction::triggered, mMainWidget->sieveEditorMainWidget(), &SieveEditorMainWidget::slotImport);
    ac->addAction(QStringLiteral("import_script"), mImportAction);
    mImportAction->setEnabled(false);

    mShareAction = new QAction(i18n("Share..."), this);
    connect(mShareAction, &QAction::triggered, mMainWidget->sieveEditorMainWidget(), &SieveEditorMainWidget::slotShareScript);
    ac->addAction(QStringLiteral("share_script"), mShareAction);
    const QStringList overlays = QStringList() << QStringLiteral("list-add");
    mShareAction->setIcon(QIcon(new KIconEngine(QStringLiteral("get-hot-new-stuff"), KIconLoader::global(), overlays)));
    mShareAction->setEnabled(false);

    mSpellCheckAction = new QAction(i18n("Check Spelling..."), this);
    connect(mSpellCheckAction, &QAction::triggered, mMainWidget->sieveEditorMainWidget(), &SieveEditorMainWidget::slotCheckSpelling);
    ac->addAction(QStringLiteral("check_spelling"), mSpellCheckAction);
    mSpellCheckAction->setIcon(QIcon::fromTheme(QStringLiteral("tools-check-spelling")));
    mSpellCheckAction->setEnabled(false);

    mCheckSyntaxAction = new QAction(i18n("Check Syntax"), this);
    connect(mCheckSyntaxAction, &QAction::triggered, mMainWidget->sieveEditorMainWidget(), &SieveEditorMainWidget::slotCheckSyntax);
    ac->addAction(QStringLiteral("check_syntax"), mCheckSyntaxAction);
    mCheckSyntaxAction->setEnabled(false);

    mCreateRulesGraphicallyAction = new QAction(i18n("Create Rules Graphically..."), this);
    connect(mCreateRulesGraphicallyAction, &QAction::triggered, mMainWidget->sieveEditorMainWidget(), &SieveEditorMainWidget::slotCreateRulesGraphically);
    ac->addAction(QStringLiteral("autogenerate_script"), mCreateRulesGraphicallyAction);
    mCreateRulesGraphicallyAction->setEnabled(false);

    mCommentAction = new QAction(i18n("Comment"), this);
    connect(mCommentAction, &QAction::triggered, mMainWidget->sieveEditorMainWidget(), &SieveEditorMainWidget::slotComment);
    ac->addAction(QStringLiteral("comment_code"), mCommentAction);
    ac->setDefaultShortcut(mCommentAction, Qt::CTRL + Qt::Key_D);

    mUncommentAction = new QAction(i18n("Uncomment"), this);
    connect(mUncommentAction, &QAction::triggered, mMainWidget->sieveEditorMainWidget(), &SieveEditorMainWidget::slotUncomment);
    ac->addAction(QStringLiteral("uncomment_code"), mUncommentAction);
    ac->setDefaultShortcut(mUncommentAction, Qt::CTRL + Qt::SHIFT + Qt::Key_D);

    mZoomInAction = new QAction(QIcon::fromTheme(QStringLiteral("zoom-in")), i18n("&Zoom In"), this);
    ac->addAction(QStringLiteral("zoom_in"), mZoomInAction);
    connect(mZoomInAction, &QAction::triggered, mMainWidget->sieveEditorMainWidget(), &SieveEditorMainWidget::slotZoomIn);
    ac->setDefaultShortcut(mZoomInAction, QKeySequence(Qt::CTRL + Qt::Key_Plus));

    mZoomOutAction = new QAction(QIcon::fromTheme(QStringLiteral("zoom-out")), i18n("Zoom &Out"), this);
    ac->addAction(QStringLiteral("zoom_out"), mZoomOutAction);
    connect(mZoomOutAction, &QAction::triggered, mMainWidget->sieveEditorMainWidget(), &SieveEditorMainWidget::slotZoomOut);
    ac->setDefaultShortcut(mZoomOutAction, QKeySequence(Qt::CTRL + Qt::Key_Minus));

    mZoomResetAction = new QAction(i18nc("Reset the zoom", "Reset"), this);
    connect(mZoomResetAction, &QAction::triggered, mMainWidget->sieveEditorMainWidget(), &SieveEditorMainWidget::slotZoomReset);
    ac->addAction(QStringLiteral("zoom_reset"), mZoomResetAction);
    ac->setDefaultShortcut(mZoomResetAction, QKeySequence(Qt::CTRL + Qt::Key_0));

    mMenuChangeCaseAction = new PimCommon::KActionMenuChangeCase(this);
    ac->addAction(QStringLiteral("change_case_menu"), mMenuChangeCaseAction);
    mMenuChangeCaseAction->appendInActionCollection(ac);
    connect(mMenuChangeCaseAction, &PimCommon::KActionMenuChangeCase::upperCase, mMainWidget->sieveEditorMainWidget(), &SieveEditorMainWidget::slotUpperCase);
    connect(mMenuChangeCaseAction, &PimCommon::KActionMenuChangeCase::lowerCase, mMainWidget->sieveEditorMainWidget(), &SieveEditorMainWidget::slotLowerCase);
    connect(mMenuChangeCaseAction, &PimCommon::KActionMenuChangeCase::sentenceCase, mMainWidget->sieveEditorMainWidget(), &SieveEditorMainWidget::slotSentenceCase);
    connect(mMenuChangeCaseAction, &PimCommon::KActionMenuChangeCase::reverseCase, mMainWidget->sieveEditorMainWidget(), &SieveEditorMainWidget::slotReverseCase);

    mBookmarkMenu = new KActionMenu(i18nc("@title:menu", "&Bookmarks"), ac);
    mSieveEditorBookmarks = new SieveEditorBookmarks(this, ac, mBookmarkMenu->menu(), this);
    ac->addAction(QStringLiteral("bookmark"), mBookmarkMenu);
    connect(mSieveEditorBookmarks, &SieveEditorBookmarks::openUrl, this, &SieveEditorMainWindow::slotOpenBookmarkUrl);

    mDebugSieveScriptAction = new QAction(i18n("Debug Sieve Script..."), this);
    connect(mDebugSieveScriptAction, &QAction::triggered, mMainWidget->sieveEditorMainWidget(), &SieveEditorMainWidget::slotDebugSieveScript);
    ac->addAction(QStringLiteral("debug_sieve"), mDebugSieveScriptAction);
    ac->setDefaultShortcut(mDebugSieveScriptAction, QKeySequence(Qt::SHIFT + Qt::ALT + Qt::Key_D));

    mWrapTextAction = new QAction(i18n("Wordwarp"), this);
    mWrapTextAction->setCheckable(true);
    ac->addAction(QStringLiteral("wordwrap"), mWrapTextAction);
    connect(mWrapTextAction, &QAction::triggered, mMainWidget->sieveEditorMainWidget(), &SieveEditorMainWidget::slotWordWrap);

    mPrintAction = KStandardAction::print(mMainWidget->sieveEditorMainWidget(), &SieveEditorMainWidget::slotPrint, ac);

    mPrintPreviewAction = KStandardAction::printPreview(mMainWidget->sieveEditorMainWidget(), &SieveEditorMainWidget::slotPrintPreview, ac);

    act = ac->addAction(QStringLiteral("import_imap_settings"), this, SLOT(slotImportImapSettings()));
    act->setText(i18n("Import IMAP Settings..."));
}

void SieveEditorMainWindow::slotImportImapSettings()
{
    //TODO open wizard
}

void SieveEditorMainWindow::slotRefreshList()
{
    if (mNetworkConfigurationManager->isOnline()) {
        mMainWidget->sieveEditorMainWidget()->refreshList();
    }
}

void SieveEditorMainWindow::slotUploadScript()
{
    mMainWidget->sieveEditorMainWidget()->uploadScript();
}

void SieveEditorMainWindow::slotDesactivateScript()
{
    mMainWidget->sieveEditorMainWidget()->desactivateScript();
}

void SieveEditorMainWindow::slotEditScript()
{
    mMainWidget->sieveEditorMainWidget()->editScript();
}

void SieveEditorMainWindow::slotCreateNewScript()
{
    mMainWidget->sieveEditorMainWidget()->createNewScript();
}

void SieveEditorMainWindow::slotDeleteScript()
{
    mMainWidget->sieveEditorMainWidget()->deleteScript();
}

void SieveEditorMainWindow::closeEvent(QCloseEvent *e)
{
    if (mMainWidget->sieveEditorMainWidget()->needToSaveScript()) {
        e->ignore();
        return;
    } else {
        e->accept();
    }
}

void SieveEditorMainWindow::slotConfigure()
{
    QPointer<SieveEditorConfigureDialog> dlg = new SieveEditorConfigureDialog(this);
    if (dlg->exec()) {
        dlg->saveServerSieveConfig();
        mMainWidget->sieveEditorMainWidget()->updateServerList();
    }
    delete dlg;
}

void SieveEditorMainWindow::slotAddServerSieve()
{
    QPointer<ServerSieveSettingsDialog> dlg = new ServerSieveSettingsDialog(this);
    if (dlg->exec()) {
        const SieveEditorUtil::SieveServerConfig conf = dlg->serverSieveConfig();
        SieveEditorUtil::addServerSieveConfig(conf);
        mMainWidget->sieveEditorMainWidget()->updateServerList();
    }
    delete dlg;
}

void SieveEditorMainWindow::slotUpdateActions()
{
    const bool hasPage = (mMainWidget->sieveEditorMainWidget()->tabWidget()->count() > 0);
    mUploadScript->setEnabled(hasPage);
    const bool editActionEnabled = (hasPage && mMainWidget->sieveEditorMainWidget()->isTextEditor());
    const bool hasActionInHtmlModeToo = (hasPage && mMainWidget->sieveEditorMainWidget()->pageMode() == KSieveUi::SieveEditorWidget::TextMode);
    mGoToLine->setEnabled(editActionEnabled);
    mFindAction->setEnabled(hasActionInHtmlModeToo);
    mReplaceAction->setEnabled(editActionEnabled);
    mUndoAction->setEnabled(editActionEnabled && mMainWidget->sieveEditorMainWidget()->isUndoAvailable());
    mRedoAction->setEnabled(editActionEnabled && mMainWidget->sieveEditorMainWidget()->isRedoAvailable());

    mCopyAction->setEnabled(hasActionInHtmlModeToo && mMainWidget->sieveEditorMainWidget()->hasSelection());
    mPasteAction->setEnabled(editActionEnabled);
    mCutAction->setEnabled(editActionEnabled && mMainWidget->sieveEditorMainWidget()->hasSelection());

    mSelectAllAction->setEnabled(hasActionInHtmlModeToo);

    mUploadScript->setEnabled(hasPage && !mNetworkIsDown);
    mRefreshList->setEnabled(!mNetworkIsDown);
    mSaveAsAction->setEnabled(hasPage);
    mImportAction->setEnabled(hasPage);
    mShareAction->setEnabled(hasPage && !mNetworkIsDown);
    mSpellCheckAction->setEnabled(editActionEnabled);
    mCheckSyntaxAction->setEnabled(editActionEnabled && !mNetworkIsDown);
    mCreateRulesGraphicallyAction->setEnabled(editActionEnabled);
    mCommentAction->setEnabled(editActionEnabled);
    mUncommentAction->setEnabled(editActionEnabled);
    mMenuChangeCaseAction->setEnabled(editActionEnabled);
    mZoomInAction->setEnabled(hasActionInHtmlModeToo);
    mZoomOutAction->setEnabled(hasActionInHtmlModeToo);
    mZoomResetAction->setEnabled(hasActionInHtmlModeToo);

    mBookmarkMenu->setEnabled(hasActionInHtmlModeToo);
    mDebugSieveScriptAction->setEnabled(editActionEnabled);
    mWrapTextAction->setEnabled(editActionEnabled);
    mWrapTextAction->setChecked(mMainWidget->sieveEditorMainWidget()->isWordWrap());

    mPrintAction->setEnabled(editActionEnabled && mMainWidget->sieveEditorMainWidget()->printSupportEnabled());
    mPrintPreviewAction->setEnabled(editActionEnabled && mMainWidget->sieveEditorMainWidget()->printSupportEnabled());
}

void SieveEditorMainWindow::slotUndoAvailable(bool b)
{
    const bool hasPage = (mMainWidget->sieveEditorMainWidget()->tabWidget()->count() > 0);
    const bool editActionEnabled = (hasPage && mMainWidget->sieveEditorMainWidget()->pageMode() == KSieveUi::SieveEditorWidget::TextMode);
    mUndoAction->setEnabled(editActionEnabled && b);
}

void SieveEditorMainWindow::slotRedoAvailable(bool b)
{
    const bool hasPage = (mMainWidget->sieveEditorMainWidget()->tabWidget()->count() > 0);
    const bool editActionEnabled = (hasPage && mMainWidget->sieveEditorMainWidget()->pageMode() == KSieveUi::SieveEditorWidget::TextMode);
    mRedoAction->setEnabled(editActionEnabled && b);
}

void SieveEditorMainWindow::slotCopyAvailable(bool b)
{
    const bool hasPage = (mMainWidget->sieveEditorMainWidget()->tabWidget()->count() > 0);
    const bool editActionEnabled = (hasPage && mMainWidget->sieveEditorMainWidget()->pageMode() == KSieveUi::SieveEditorWidget::TextMode);
    mCopyAction->setEnabled(editActionEnabled && b);
    mCutAction->setEnabled(editActionEnabled && b);
}

void SieveEditorMainWindow::slotOpenBookmarkUrl(const QUrl &url)
{
    mMainWidget->sieveEditorMainWidget()->openBookmarkUrl(url);
}

QString SieveEditorMainWindow::currentHelpTitle() const
{
    return mMainWidget->sieveEditorMainWidget()->currentHelpTitle();
}

QUrl SieveEditorMainWindow::currentHelpUrl() const
{
    return mMainWidget->sieveEditorMainWidget()->currentHelpUrl();
}
