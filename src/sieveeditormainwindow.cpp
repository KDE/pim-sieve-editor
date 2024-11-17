/*
   SPDX-FileCopyrightText: 2013-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "sieveeditormainwindow.h"
#include "config-pim-sieve-editor.h"
#include "importwizard/importimapsettingwizard.h"
#include "serversievesettingsdialog.h"
#include "sieveeditorbookmarks.h"
#include "sieveeditorcentralwidget.h"
#include "sieveeditorconfiguredialog.h"
#include "sieveeditorglobalconfig.h"
#include "sieveeditormainwidget.h"
#include "sievepurposemenuwidget.h"
#include <PimCommon/KActionMenuChangeCase>
#include <PimCommon/NeedUpdateVersionUtils>
#include <PimCommon/NeedUpdateVersionWidget>

#include <KSharedConfig>
#include <PimCommon/NetworkManager>

#include <KAboutData>
#include <KActionCollection>
#include <KColorSchemeMenu>
#include <KConfigGroup>
#include <KLocalizedString>
#include <KMessageBox>
#include <KStandardAction>
#include <KToggleFullScreenAction>
#include <KToolBar>
#include <QVBoxLayout>

#include <KColorSchemeManager>
#include <QAction>
#include <QCloseEvent>
#include <QFontDatabase>
#include <QIcon>
#include <QLabel>
#include <QMenu>
#include <QMenuBar>
#include <QPointer>
#include <QStatusBar>
#include <QToolButton>
#if WITH_KUSERFEEDBACK
#include "userfeedback/userfeedbackmanager.h"
#include <KUserFeedback/NotificationPopup>
#include <KUserFeedback/Provider>
#endif
// signal handler for SIGINT & SIGTERM
#ifdef Q_OS_UNIX
#include <KSignalHandler>
#include <signal.h>
#include <unistd.h>
#endif
#include <KWindowConfig>
#include <KWindowSystem>
#include <QWindow>
#if defined(Q_OS_WIN) || defined(Q_OS_MACOS)
#include <PimCommon/VerifyNewVersionWidget>
#endif

namespace
{
static const char mySieveEditorMainWindowConfigGroupName[] = "SieveEditorMainWindow";
}
SieveEditorMainWindow::SieveEditorMainWindow(QWidget *parent)
    : KXmlGuiWindow(parent)
#if defined(Q_OS_WIN) || defined(Q_OS_MACOS)
    , mVerifyNewVersionWidget(new PimCommon::VerifyNewVersionWidget(this))
#endif

{
#ifdef Q_OS_UNIX
    /**
     * Set up signal handler for SIGINT and SIGTERM
     */
    KSignalHandler::self()->watchSignal(SIGINT);
    KSignalHandler::self()->watchSignal(SIGTERM);
    connect(KSignalHandler::self(), &KSignalHandler::signalReceived, this, [this](int signal) {
        if (signal == SIGINT || signal == SIGTERM) {
            printf("Shutting down…\n");
            close();
        }
    });
#endif

    auto mainWidget = new QWidget(this);
    auto mainWidgetLayout = new QVBoxLayout(mainWidget);
    mainWidgetLayout->setContentsMargins({});
    mainWidgetLayout->setSpacing(0);
    if (PimCommon::NeedUpdateVersionUtils::checkVersion()) {
        const auto status = PimCommon::NeedUpdateVersionUtils::obsoleteVersionStatus(KAboutData::applicationData().version(), QDate::currentDate());
        if (status != PimCommon::NeedUpdateVersionUtils::ObsoleteVersion::NotObsoleteYet) {
            auto needUpdateVersionWidget = new PimCommon::NeedUpdateVersionWidget(this);
            mainWidgetLayout->addWidget(needUpdateVersionWidget);
            needUpdateVersionWidget->setObsoleteVersion(status);
        }
    }

    mMainWidget = new SieveEditorCentralWidget(this, actionCollection());
    mainWidgetLayout->addWidget(mMainWidget);
#if WITH_KUSERFEEDBACK
    // Initialize
    (void)UserFeedBackManager::self();
#endif
    connect(mMainWidget, &SieveEditorCentralWidget::configureClicked, this, &SieveEditorMainWindow::slotConfigure);
    connect(mMainWidget, &SieveEditorCentralWidget::importSieveSettings, this, &SieveEditorMainWindow::slotImportImapSettings);
    connect(mMainWidget->sieveEditorMainWidget(), &SieveEditorMainWidget::updateButtons, this, &SieveEditorMainWindow::slotUpdateButtons);
    setCentralWidget(mainWidget);
    setupActions();
    setupGUI();
    readConfig();
    initStatusBar();
    connect(PimCommon::NetworkManager::self(),
            &PimCommon::NetworkManager::networkStatusChanged,
            this,
            &SieveEditorMainWindow::slotSystemNetworkOnlineStateChanged);

    connect(mMainWidget->sieveEditorMainWidget()->tabWidget(), &QTabWidget::currentChanged, this, &SieveEditorMainWindow::slotUpdateActions);
    connect(mMainWidget->sieveEditorMainWidget(), &SieveEditorMainWidget::modeEditorChanged, this, &SieveEditorMainWindow::slotUpdateActions);
    slotSystemNetworkOnlineStateChanged(PimCommon::NetworkManager::self()->isOnline());
    connect(mMainWidget->sieveEditorMainWidget(), &SieveEditorMainWidget::undoAvailable, this, &SieveEditorMainWindow::slotUndoAvailable);
    connect(mMainWidget->sieveEditorMainWidget(), &SieveEditorMainWidget::redoAvailable, this, &SieveEditorMainWindow::slotRedoAvailable);
    connect(mMainWidget->sieveEditorMainWidget(), &SieveEditorMainWidget::copyAvailable, this, &SieveEditorMainWindow::slotCopyAvailable);
    connect(mMainWidget->sieveEditorMainWidget(), &SieveEditorMainWidget::sieveEditorTabCurrentChanged, this, &SieveEditorMainWindow::slotUpdateActions);
    mMainWidget->sieveEditorMainWidget()->refreshList();
#if WITH_KUSERFEEDBACK
    auto userFeedBackNotificationPopup = new KUserFeedback::NotificationPopup(this);
    userFeedBackNotificationPopup->setFeedbackProvider(UserFeedBackManager::self()->userFeedbackProvider());
#endif
}

SieveEditorMainWindow::~SieveEditorMainWindow()
{
    writeConfig();
}

void SieveEditorMainWindow::readConfig()
{
    create(); // ensure a window is created
    windowHandle()->resize(QSize(800, 600));
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(mySieveEditorMainWindowConfigGroupName));
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void SieveEditorMainWindow::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(mySieveEditorMainWindowConfigGroupName));
    KWindowConfig::saveWindowSize(windowHandle(), group);
    group.sync();
}

void SieveEditorMainWindow::initStatusBar()
{
    mStatusBarInfo = new QLabel;
    mStatusBarInfo->setTextFormat(Qt::PlainText);
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

void SieveEditorMainWindow::setupActions()
{
    KActionCollection *ac = actionCollection();

    KStandardActions::quit(this, &SieveEditorMainWindow::close, ac);
    KStandardActions::preferences(this, &SieveEditorMainWindow::slotConfigure, ac);

    mUploadScript = KStandardActions::save(this, &SieveEditorMainWindow::slotUploadScript, ac);
    mUploadScript->setText(i18n("Upload"));
    mUploadScript->setEnabled(false);

    auto act = new QAction(i18nc("@action", "Add Sieve Server…"), this);
    connect(act, &QAction::triggered, this, &SieveEditorMainWindow::slotAddServerSieve);
    ac->addAction(QStringLiteral("add_server_sieve"), act);

    mDeleteScript = new QAction(i18nc("@action", "Delete Script"), this);
    connect(mDeleteScript, &QAction::triggered, this, &SieveEditorMainWindow::slotDeleteScript);
    ac->addAction(QStringLiteral("delete_script"), mDeleteScript);
    ac->setDefaultShortcut(mDeleteScript, QKeySequence(Qt::Key_Delete));
    mDeleteScript->setEnabled(false);

    mNewScript = new QAction(i18nc("@action", "Create New Script…"), this);
    connect(mNewScript, &QAction::triggered, this, &SieveEditorMainWindow::slotCreateNewScript);
    ac->addAction(QStringLiteral("create_new_script"), mNewScript);
    ac->setDefaultShortcut(mNewScript, QKeySequence(Qt::CTRL | Qt::Key_N));
    mNewScript->setEnabled(false);

    mEditScript = new QAction(i18nc("@action", "Edit Script…"), this);
    connect(mEditScript, &QAction::triggered, this, &SieveEditorMainWindow::slotEditScript);
    ac->addAction(QStringLiteral("edit_script"), mEditScript);
    mEditScript->setEnabled(false);

    mDesactivateScript = new QAction(i18nc("@action", "Deactivate Script"), this);
    connect(mDesactivateScript, &QAction::triggered, this, &SieveEditorMainWindow::slotDesactivateScript);
    ac->addAction(QStringLiteral("desactivate_script"), mDesactivateScript);
    mDesactivateScript->setEnabled(false);

    mRefreshList = new QAction(i18nc("@action", "Refresh List"), this);
    connect(mRefreshList, &QAction::triggered, this, &SieveEditorMainWindow::slotRefreshList);
    ac->addAction(QStringLiteral("refresh_list"), mRefreshList);
    mRefreshList->setIcon(QIcon::fromTheme(QStringLiteral("view-refresh")));
    ac->setDefaultShortcut(mRefreshList, QKeySequence(Qt::Key_F5));

    mGoToLine = new QAction(i18nc("@action", "Go to Line…"), this);
    connect(mGoToLine, &QAction::triggered, mMainWidget->sieveEditorMainWidget(), &SieveEditorMainWidget::slotGoToLine);
    mGoToLine->setIcon(QIcon::fromTheme(QStringLiteral("go-jump")));
    ac->setDefaultShortcut(mGoToLine, QKeySequence(Qt::CTRL | Qt::Key_G));
    ac->addAction(QStringLiteral("gotoline"), mGoToLine);
    mGoToLine->setEnabled(false);

    mFindAction = KStandardActions::find(mMainWidget->sieveEditorMainWidget(), &SieveEditorMainWidget::slotFind, ac);
    mReplaceAction = KStandardActions::replace(mMainWidget->sieveEditorMainWidget(), &SieveEditorMainWidget::slotReplace, ac);
    mUndoAction = KStandardActions::undo(mMainWidget->sieveEditorMainWidget(), &SieveEditorMainWidget::slotUndo, ac);
    mRedoAction = KStandardActions::redo(mMainWidget->sieveEditorMainWidget(), &SieveEditorMainWidget::slotRedo, ac);
    mCopyAction = KStandardActions::copy(mMainWidget->sieveEditorMainWidget(), &SieveEditorMainWidget::slotCopy, ac);
    mPasteAction = KStandardActions::paste(mMainWidget->sieveEditorMainWidget(), &SieveEditorMainWidget::slotPaste, ac);
    mCutAction = KStandardActions::cut(mMainWidget->sieveEditorMainWidget(), &SieveEditorMainWidget::slotCut, ac);
    mSelectAllAction = KStandardActions::selectAll(mMainWidget->sieveEditorMainWidget(), &SieveEditorMainWidget::slotSelectAll, ac);
    mSaveAsAction = KStandardActions::saveAs(mMainWidget->sieveEditorMainWidget(), &SieveEditorMainWidget::slotSaveAs, ac);

    mImportAction = new QAction(i18nc("@action", "Import…"), this);
    connect(mImportAction, &QAction::triggered, mMainWidget->sieveEditorMainWidget(), &SieveEditorMainWidget::slotImport);
    ac->addAction(QStringLiteral("import_script"), mImportAction);
    mImportAction->setEnabled(false);

    auto purposeMenu = new SievePurposeMenuWidget(this, this);
    mShareAction = new QAction(i18nc("@action", "Share…"), this);
    ac->addAction(QStringLiteral("share_script"), mShareAction);
    mShareAction->setMenu(purposeMenu->menu());
    mShareAction->setIcon(QIcon::fromTheme(QStringLiteral("document-share")));
    purposeMenu->setEditorWidget(mMainWidget->sieveEditorMainWidget());
    connect(purposeMenu, &SievePurposeMenuWidget::shareError, mMainWidget, &SieveEditorCentralWidget::slotShareError);
    connect(purposeMenu, &SievePurposeMenuWidget::shareSuccess, mMainWidget, &SieveEditorCentralWidget::slotShareSuccess);

    mSpellCheckAction = new QAction(i18nc("@action", "Check Spelling…"), this);
    connect(mSpellCheckAction, &QAction::triggered, mMainWidget->sieveEditorMainWidget(), &SieveEditorMainWidget::slotCheckSpelling);
    ac->addAction(QStringLiteral("check_spelling"), mSpellCheckAction);
    mSpellCheckAction->setIcon(QIcon::fromTheme(QStringLiteral("tools-check-spelling")));
    mSpellCheckAction->setEnabled(false);

    mCheckSyntaxAction = new QAction(i18nc("@action", "Check Syntax"), this);
    connect(mCheckSyntaxAction, &QAction::triggered, mMainWidget->sieveEditorMainWidget(), &SieveEditorMainWidget::slotCheckSyntax);
    ac->addAction(QStringLiteral("check_syntax"), mCheckSyntaxAction);
    mCheckSyntaxAction->setEnabled(false);

    mCreateRulesGraphicallyAction = new QAction(i18nc("@action", "Create Rules Graphically…"), this);
    connect(mCreateRulesGraphicallyAction, &QAction::triggered, mMainWidget->sieveEditorMainWidget(), &SieveEditorMainWidget::slotCreateRulesGraphically);
    ac->addAction(QStringLiteral("autogenerate_script"), mCreateRulesGraphicallyAction);
    mCreateRulesGraphicallyAction->setEnabled(false);

    mCommentAction = new QAction(i18nc("@action", "Comment"), this);
    connect(mCommentAction, &QAction::triggered, mMainWidget->sieveEditorMainWidget(), &SieveEditorMainWidget::slotComment);
    ac->addAction(QStringLiteral("comment_code"), mCommentAction);
    ac->setDefaultShortcut(mCommentAction, Qt::CTRL | Qt::Key_D);

    mUncommentAction = new QAction(i18nc("@action", "Uncomment"), this);
    connect(mUncommentAction, &QAction::triggered, mMainWidget->sieveEditorMainWidget(), &SieveEditorMainWidget::slotUncomment);
    ac->addAction(QStringLiteral("uncomment_code"), mUncommentAction);
    ac->setDefaultShortcut(mUncommentAction, Qt::CTRL | Qt::SHIFT | Qt::Key_D);

    mZoomInAction = KStandardActions::zoomIn(mMainWidget->sieveEditorMainWidget(), &SieveEditorMainWidget::slotZoomIn, this);
    ac->addAction(QStringLiteral("zoom_in"), mZoomInAction);

    mZoomOutAction = KStandardActions::zoomOut(mMainWidget->sieveEditorMainWidget(), &SieveEditorMainWidget::slotZoomOut, this);
    ac->addAction(QStringLiteral("zoom_out"), mZoomOutAction);

    mZoomResetAction = KStandardActions::actualSize(mMainWidget->sieveEditorMainWidget(), &SieveEditorMainWidget::slotZoomReset, this);
    ac->addAction(QStringLiteral("zoom_reset"), mZoomResetAction);

    mMenuChangeCaseAction = new PimCommon::KActionMenuChangeCase(this);
    ac->addAction(QStringLiteral("change_case_menu"), mMenuChangeCaseAction);
    mMenuChangeCaseAction->appendInActionCollection(ac);
    connect(mMenuChangeCaseAction, &PimCommon::KActionMenuChangeCase::upperCase, mMainWidget->sieveEditorMainWidget(), &SieveEditorMainWidget::slotUpperCase);
    connect(mMenuChangeCaseAction, &PimCommon::KActionMenuChangeCase::lowerCase, mMainWidget->sieveEditorMainWidget(), &SieveEditorMainWidget::slotLowerCase);
    connect(mMenuChangeCaseAction,
            &PimCommon::KActionMenuChangeCase::sentenceCase,
            mMainWidget->sieveEditorMainWidget(),
            &SieveEditorMainWidget::slotSentenceCase);
    connect(mMenuChangeCaseAction,
            &PimCommon::KActionMenuChangeCase::reverseCase,
            mMainWidget->sieveEditorMainWidget(),
            &SieveEditorMainWidget::slotReverseCase);

    mBookmarkMenu = new KActionMenu(i18nc("@title:menu", "&Bookmarks"), ac);
    mSieveEditorBookmarks = new SieveEditorBookmarks(this, ac, mBookmarkMenu->menu(), this);
    ac->addAction(QStringLiteral("bookmark"), mBookmarkMenu);
    connect(mSieveEditorBookmarks, &SieveEditorBookmarks::openUrl, this, &SieveEditorMainWindow::slotOpenBookmarkUrl);

    mDebugSieveScriptAction = new QAction(i18nc("@action", "Debug Sieve Script…"), this);
    connect(mDebugSieveScriptAction, &QAction::triggered, mMainWidget->sieveEditorMainWidget(), &SieveEditorMainWidget::slotDebugSieveScript);
    ac->addAction(QStringLiteral("debug_sieve"), mDebugSieveScriptAction);
    ac->setDefaultShortcut(mDebugSieveScriptAction, QKeySequence(Qt::SHIFT | Qt::ALT | Qt::Key_D));

    mWrapTextAction = new QAction(i18nc("@action", "Wordwrap"), this);
    mWrapTextAction->setCheckable(true);
    ac->addAction(QStringLiteral("wordwrap"), mWrapTextAction);
    connect(mWrapTextAction, &QAction::triggered, mMainWidget->sieveEditorMainWidget(), &SieveEditorMainWidget::slotWordWrap);

    mPrintAction = KStandardActions::print(mMainWidget->sieveEditorMainWidget(), &SieveEditorMainWidget::slotPrint, ac);

    mShowMenuBarAction = KStandardAction::showMenubar(this, &SieveEditorMainWindow::slotToggleMenubar, actionCollection());

    mPrintPreviewAction = KStandardActions::printPreview(mMainWidget->sieveEditorMainWidget(), &SieveEditorMainWidget::slotPrintPreview, ac);
    if (menuBar()) {
        mHamburgerMenu = KStandardAction::hamburgerMenu(nullptr, nullptr, actionCollection());
        mHamburgerMenu->setShowMenuBarAction(mShowMenuBarAction);
        mHamburgerMenu->setMenuBar(menuBar());
        connect(mHamburgerMenu, &KHamburgerMenu::aboutToShowMenu, this, [this]() {
            updateHamburgerMenu();
            // Immediately disconnect. We only need to run this once, but on demand.
            // NOTE: The nullptr at the end disconnects all connections between
            // q and mHamburgerMenu's aboutToShowMenu signal.
            disconnect(mHamburgerMenu, &KHamburgerMenu::aboutToShowMenu, this, nullptr);
        });
    }
    act = new QAction(i18nc("@action", "Import IMAP Settings…"), this);
    ac->addAction(QStringLiteral("import_imap_settings"), act);
    connect(act, &QAction::triggered, this, &SieveEditorMainWindow::slotImportImapSettings);

    mShowFullScreenAction = KStandardAction::fullScreen(nullptr, nullptr, this, ac);
    ac->setDefaultShortcut(mShowFullScreenAction, Qt::Key_F11);
    connect(mShowFullScreenAction, &QAction::toggled, this, &SieveEditorMainWindow::slotFullScreen);

#if defined(Q_OS_WIN) || defined(Q_OS_MACOS)
    mVerifyNewVersionWidget->addOsUrlInfo(PimCommon::VerifyNewVersionWidget::OsVersion::Windows,
                                          QStringLiteral("https://cdn.kde.org/ci-builds/pim/pim-sieve-editor"));
    auto verifyNewVersionAction = mVerifyNewVersionWidget->verifyNewVersionAction();
    ac->addAction(QStringLiteral("verify_check_version"), verifyNewVersionAction);
#endif

    auto manager = KColorSchemeManager::instance();
    ac->addAction(QStringLiteral("colorscheme_menu"), KColorSchemeMenu::createMenu(manager, this));
    mShowMenuBarAction->setChecked(SieveEditorGlobalConfig::self()->showMenuBar());
    slotToggleMenubar(true);
}

void SieveEditorMainWindow::slotActivateRequested(const QStringList &arguments, const QString &workingDirectory)
{
    Q_UNUSED(workingDirectory)
    // TODO arguments
#if !defined(Q_OS_WIN) && !defined(Q_OS_MACOS)
    KWindowSystem::updateStartupId(windowHandle());
    KWindowSystem::activateWindow(windowHandle());
#else
    activateWindow();
#endif
}

void SieveEditorMainWindow::slotToggleMenubar(bool dontShowWarning)
{
    if (menuBar()) {
        if (mShowMenuBarAction->isChecked()) {
            menuBar()->show();
        } else {
            if (!dontShowWarning && (!toolBar()->isVisible() || !toolBar()->actions().contains(mHamburgerMenu))) {
                const QString accel = mShowMenuBarAction->shortcut().toString(QKeySequence::NativeText);
                KMessageBox::information(this,
                                         i18n("<qt>This will hide the menu bar completely."
                                              " You can show it again by typing %1.</qt>",
                                              accel),
                                         i18nc("@title:window", "Hide menu bar"),
                                         QStringLiteral("HideMenuBarWarning"));
            }
            menuBar()->hide();
        }
        SieveEditorGlobalConfig::self()->setShowMenuBar(mShowMenuBarAction->isChecked());
        SieveEditorGlobalConfig::self()->save();
    }
}

void SieveEditorMainWindow::updateHamburgerMenu()
{
    delete mHamburgerMenu->menu();
    auto menu = new QMenu(this);

    menu->addAction(actionCollection()->action(QStringLiteral("add_server_sieve")));
    menu->addSeparator();
    menu->addAction(actionCollection()->action(QStringLiteral("create_new_script")));
    menu->addSeparator();
    mHamburgerMenu->setMenu(menu);
}

void SieveEditorMainWindow::slotImportImapSettings()
{
    QPointer<ImportImapSettingWizard> w = new ImportImapSettingWizard(this);
    if (w->exec()) {
        mMainWidget->sieveEditorMainWidget()->updateServerList();
    }
    delete w;
}

void SieveEditorMainWindow::slotRefreshList()
{
    if (PimCommon::NetworkManager::self()->isOnline()) {
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

QString SieveEditorMainWindow::currentText() const
{
    return mMainWidget->sieveEditorMainWidget()->currentText();
}

void SieveEditorMainWindow::slotFullScreen(bool t)
{
    KToggleFullScreenAction::setFullScreen(this, t);
    QMenuBar *mb = menuBar();
    if (t) {
        auto b = new QToolButton(mb);
        b->setDefaultAction(mShowFullScreenAction);
        b->setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Ignored));
        b->setFont(QFontDatabase::systemFont(QFontDatabase::SmallestReadableFont));
        mb->setCornerWidget(b, Qt::TopRightCorner);
        b->setVisible(true);
        b->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    } else {
        QWidget *w = mb->cornerWidget(Qt::TopRightCorner);
        if (w) {
            w->deleteLater();
        }
    }
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

#include "moc_sieveeditormainwindow.cpp"
