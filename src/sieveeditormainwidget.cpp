/*
   SPDX-FileCopyrightText: 2013-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "sieveeditormainwidget.h"
using namespace Qt::Literals::StringLiterals;

#include "sieveeditoremptytabwidgetlabel.h"
#include "sieveeditorpagewidget.h"
#include "sieveeditorscriptmanagerwidget.h"
#include "sieveeditortabwidget.h"
#include <KSieveUi/SieveEditor>

#include <KColorScheme>
#include <KConfigGroup>
#include <KLocalizedString>
#include <KMessageBox>
#include <KSharedConfig>
#include <QTabWidget>

#include <QEvent>
#include <QStackedWidget>
#include <QTabBar>

namespace
{
static const char mySieveEditorMainWidgetConfigGroupName[] = "SieveEditorMainWidget";
}
SieveEditorMainWidget::SieveEditorMainWidget(KActionCollection *ac, QWidget *parent)
    : QSplitter(parent)
    , mTabWidget(new SieveEditorTabWidget(ac, this))
    , mScriptManagerWidget(new SieveEditorScriptManagerWidget(this))
    , mStackedWidget(new QStackedWidget(this))
    , mEditorEmptyLabel(new SieveEditorEmptyTabWidgetLabel(this))
{
    mStackedWidget->setObjectName("stackedwidget"_L1);

    connect(mTabWidget, &SieveEditorTabWidget::tabCloseRequestedIndex, this, &SieveEditorMainWidget::slotTabCloseRequested);
    connect(mTabWidget, &SieveEditorTabWidget::tabRemoveAllExclude, this, &SieveEditorMainWidget::slotTabRemoveAllExclude);
    connect(mTabWidget, &SieveEditorTabWidget::tabCloseAllTab, this, &SieveEditorMainWidget::slotTabCloseAllRequested);

    mStackedWidget->addWidget(mTabWidget);
    mStackedWidget->addWidget(mEditorEmptyLabel);
    addWidget(mStackedWidget);

    connect(mScriptManagerWidget, &SieveEditorScriptManagerWidget::createScriptPage, this, &SieveEditorMainWidget::slotCreateScriptPage);
    connect(mScriptManagerWidget, &SieveEditorScriptManagerWidget::updateButtons, this, &SieveEditorMainWidget::updateButtons);
    connect(mScriptManagerWidget, &SieveEditorScriptManagerWidget::scriptDeleted, this, &SieveEditorMainWidget::slotScriptDeleted);
    connect(mScriptManagerWidget, &SieveEditorScriptManagerWidget::serverSieveFound, this, &SieveEditorMainWidget::serverSieveFound);
    connect(mScriptManagerWidget, &SieveEditorScriptManagerWidget::scriptRenamed, this, &SieveEditorMainWidget::slotScriptRenamed);
    connect(this, &SieveEditorMainWidget::updateScriptList, mScriptManagerWidget, &SieveEditorScriptManagerWidget::slotRefreshList);
    addWidget(mScriptManagerWidget);
    setChildrenCollapsible(false);
    QList<int> splitterSizes;
    splitterSizes << 80 << 20;
    KConfigGroup myGroup(KSharedConfig::openStateConfig(), QLatin1StringView(mySieveEditorMainWidgetConfigGroupName));
    setSizes(myGroup.readEntry("mainSplitter", splitterSizes));
    updateStackedWidget();
}

SieveEditorMainWidget::~SieveEditorMainWidget()
{
    disconnect(mScriptManagerWidget, &SieveEditorScriptManagerWidget::updateButtons, this, &SieveEditorMainWidget::updateButtons);
    KConfigGroup myGroup(KSharedConfig::openStateConfig(), QLatin1StringView(mySieveEditorMainWidgetConfigGroupName));
    myGroup.writeEntry("mainSplitter", sizes());
    myGroup.sync();
}

QWidget *SieveEditorMainWidget::hasExistingPage(const QUrl &url)
{
    for (int i = 0; i < mTabWidget->count(); ++i) {
        auto page = qobject_cast<SieveEditorPageWidget *>(mTabWidget->widget(i));
        if (page) {
            if (page->currentUrl() == url) {
                return page;
            }
        }
    }
    return nullptr;
}

void SieveEditorMainWidget::updateStackedWidget()
{
    if (mTabWidget->count() == 0) {
        mStackedWidget->setCurrentWidget(mEditorEmptyLabel);
    } else {
        mStackedWidget->setCurrentWidget(mTabWidget);
    }
}

void SieveEditorMainWidget::slotScriptDeleted(const QUrl &url)
{
    QWidget *page = hasExistingPage(url);
    if (page) {
        mTabWidget->removeTab(mTabWidget->indexOf(page));
        delete page;
    }
    updateStackedWidget();
}

void SieveEditorMainWidget::slotScriptRenamed(const QUrl &oldUrl, const QUrl &newUrl)
{
    QWidget *page = hasExistingPage(oldUrl);
    if (page) {
        if (auto editor = qobject_cast<SieveEditorPageWidget *>(page)) {
            editor->setCurrentUrl(newUrl);
            const QString newScriptName = newUrl.fileName();
            mTabWidget->setTabText(mTabWidget->indexOf(page), newScriptName);
            editor->renameScriptName(newScriptName);
        }
    }
    updateStackedWidget();
}

void SieveEditorMainWidget::slotCreateScriptPage(const KSieveUi::ManageSieveWidget::ScriptInfo &info, bool isNewScript)
{
    QWidget *page = hasExistingPage(info.currentUrl);
    if (page) {
        mTabWidget->setCurrentWidget(page);
    } else {
        auto editor = new SieveEditorPageWidget;
        connect(editor, &SieveEditorPageWidget::refreshList, this, &SieveEditorMainWidget::updateScriptList);
        connect(editor, &SieveEditorPageWidget::scriptModified, this, &SieveEditorMainWidget::slotScriptModified);
        connect(editor, &SieveEditorPageWidget::modeEditorChanged, this, &SieveEditorMainWidget::modeEditorChanged);
        connect(editor, &SieveEditorPageWidget::undoAvailable, this, &SieveEditorMainWidget::undoAvailable);
        connect(editor, &SieveEditorPageWidget::redoAvailable, this, &SieveEditorMainWidget::redoAvailable);
        connect(editor, &SieveEditorPageWidget::copyAvailable, this, &SieveEditorMainWidget::copyAvailable);
        connect(editor, &SieveEditorPageWidget::sieveEditorTabCurrentChanged, this, &SieveEditorMainWidget::sieveEditorTabCurrentChanged);
        connect(editor, &SieveEditorPageWidget::requestCloseTab, this, &SieveEditorMainWidget::forceCloseTab);
        editor->setIsNewScript(isNewScript);
        editor->loadScript(info);
        mTabWidget->addTab(editor, info.currentUrl.fileName());
        mTabWidget->setCurrentWidget(editor);
        if (isNewScript) {
            editor->uploadScript(false, true);
        }
        updateStackedWidget();
    }
}

void SieveEditorMainWidget::createNewScript()
{
    mScriptManagerWidget->slotCreateNewScript();
}

void SieveEditorMainWidget::deleteScript()
{
    mScriptManagerWidget->slotDeleteScript();
}

void SieveEditorMainWidget::updateServerList()
{
    mScriptManagerWidget->updateServerList();
}

void SieveEditorMainWidget::editScript()
{
    mScriptManagerWidget->editScript();
}

void SieveEditorMainWidget::desactivateScript()
{
    mScriptManagerWidget->desactivateScript();
}

void SieveEditorMainWidget::refreshList()
{
    mScriptManagerWidget->refreshList();
}

void SieveEditorMainWidget::uploadScript()
{
    QWidget *w = mTabWidget->currentWidget();
    if (w) {
        auto page = qobject_cast<SieveEditorPageWidget *>(w);
        if (page) {
            page->uploadScript();
        }
    }
}

bool SieveEditorMainWidget::needToSaveScript()
{
    bool scriptSaved = false;
    for (int i = 0; i < mTabWidget->count(); ++i) {
        auto page = qobject_cast<SieveEditorPageWidget *>(mTabWidget->widget(i));
        if (page) {
            const bool result = page->needToSaveScript();
            if (result) {
                scriptSaved = true;
            }
        }
    }
    return scriptSaved;
}

QTabWidget *SieveEditorMainWidget::tabWidget() const
{
    return mTabWidget;
}

bool SieveEditorMainWidget::isUndoAvailable() const
{
    QWidget *w = mTabWidget->currentWidget();
    if (w) {
        auto page = qobject_cast<SieveEditorPageWidget *>(w);
        if (page) {
            return page->isUndoAvailable();
        }
    }
    return false;
}

bool SieveEditorMainWidget::isRedoAvailable() const
{
    QWidget *w = mTabWidget->currentWidget();
    if (w) {
        auto page = qobject_cast<SieveEditorPageWidget *>(w);
        if (page) {
            return page->isRedoAvailable();
        }
    }
    return false;
}

bool SieveEditorMainWidget::isWordWrap() const
{
    QWidget *w = mTabWidget->currentWidget();
    if (w) {
        auto page = qobject_cast<SieveEditorPageWidget *>(w);
        if (page) {
            return page->isWordWrap();
        }
    }
    return false;
}

bool SieveEditorMainWidget::hasSelection() const
{
    QWidget *w = mTabWidget->currentWidget();
    if (w) {
        auto page = qobject_cast<SieveEditorPageWidget *>(w);
        if (page) {
            return page->hasSelection();
        }
    }
    return false;
}

void SieveEditorMainWidget::slotSelectAll()
{
    QWidget *w = mTabWidget->currentWidget();
    if (w) {
        auto page = qobject_cast<SieveEditorPageWidget *>(w);
        if (page) {
            page->selectAll();
        }
    }
}

void SieveEditorMainWidget::slotCopy()
{
    QWidget *w = mTabWidget->currentWidget();
    if (w) {
        auto page = qobject_cast<SieveEditorPageWidget *>(w);
        if (page) {
            page->copy();
        }
    }
}

void SieveEditorMainWidget::slotPaste()
{
    QWidget *w = mTabWidget->currentWidget();
    if (w) {
        auto page = qobject_cast<SieveEditorPageWidget *>(w);
        if (page) {
            page->paste();
        }
    }
}

void SieveEditorMainWidget::slotCut()
{
    QWidget *w = mTabWidget->currentWidget();
    if (w) {
        auto page = qobject_cast<SieveEditorPageWidget *>(w);
        if (page) {
            page->cut();
        }
    }
}

void SieveEditorMainWidget::slotUndo()
{
    QWidget *w = mTabWidget->currentWidget();
    if (w) {
        auto page = qobject_cast<SieveEditorPageWidget *>(w);
        if (page) {
            page->undo();
        }
    }
}

void SieveEditorMainWidget::slotRedo()
{
    QWidget *w = mTabWidget->currentWidget();
    if (w) {
        auto page = qobject_cast<SieveEditorPageWidget *>(w);
        if (page) {
            page->redo();
        }
    }
}

void SieveEditorMainWidget::slotGoToLine()
{
    QWidget *w = mTabWidget->currentWidget();
    if (w) {
        auto page = qobject_cast<SieveEditorPageWidget *>(w);
        if (page) {
            page->goToLine();
        }
    }
}

void SieveEditorMainWidget::slotFind()
{
    QWidget *w = mTabWidget->currentWidget();
    if (w) {
        auto page = qobject_cast<SieveEditorPageWidget *>(w);
        if (page) {
            page->find();
        }
    }
}

void SieveEditorMainWidget::slotReplace()
{
    QWidget *w = mTabWidget->currentWidget();
    if (w) {
        auto page = qobject_cast<SieveEditorPageWidget *>(w);
        if (page) {
            page->replace();
        }
    }
}

QString SieveEditorMainWidget::currentText() const
{
    QWidget *w = mTabWidget->currentWidget();
    if (w) {
        auto page = qobject_cast<SieveEditorPageWidget *>(w);
        if (page) {
            return page->currentText();
        }
    }
    return {};
}

void SieveEditorMainWidget::slotCreateRulesGraphically()
{
    QWidget *w = mTabWidget->currentWidget();
    if (w) {
        auto page = qobject_cast<SieveEditorPageWidget *>(w);
        if (page) {
            page->createRulesGraphically();
        }
    }
}

void SieveEditorMainWidget::slotCheckSyntax()
{
    QWidget *w = mTabWidget->currentWidget();
    if (w) {
        auto page = qobject_cast<SieveEditorPageWidget *>(w);
        if (page) {
            page->checkSyntax();
        }
    }
}

void SieveEditorMainWidget::slotComment()
{
    QWidget *w = mTabWidget->currentWidget();
    if (w) {
        auto page = qobject_cast<SieveEditorPageWidget *>(w);
        if (page) {
            page->comment();
        }
    }
}

void SieveEditorMainWidget::slotUncomment()
{
    QWidget *w = mTabWidget->currentWidget();
    if (w) {
        auto page = qobject_cast<SieveEditorPageWidget *>(w);
        if (page) {
            page->uncomment();
        }
    }
}

void SieveEditorMainWidget::slotImport()
{
    QWidget *w = mTabWidget->currentWidget();
    if (w) {
        auto page = qobject_cast<SieveEditorPageWidget *>(w);
        if (page) {
            page->import();
        }
    }
}

void SieveEditorMainWidget::slotCheckSpelling()
{
    QWidget *w = mTabWidget->currentWidget();
    if (w) {
        auto page = qobject_cast<SieveEditorPageWidget *>(w);
        if (page) {
            page->checkSpelling();
        }
    }
}

void SieveEditorMainWidget::slotSaveAs()
{
    QWidget *w = mTabWidget->currentWidget();
    if (w) {
        auto page = qobject_cast<SieveEditorPageWidget *>(w);
        if (page) {
            page->saveAs();
        }
    }
}

void SieveEditorMainWidget::slotReverseCase()
{
    QWidget *w = mTabWidget->currentWidget();
    if (w) {
        auto page = qobject_cast<SieveEditorPageWidget *>(w);
        if (page) {
            page->reverseCase();
        }
    }
}

void SieveEditorMainWidget::slotDebugSieveScript()
{
    QWidget *w = mTabWidget->currentWidget();
    if (w) {
        auto page = qobject_cast<SieveEditorPageWidget *>(w);
        if (page) {
            page->debugSieveScript();
        }
    }
}

void SieveEditorMainWidget::slotUpperCase()
{
    QWidget *w = mTabWidget->currentWidget();
    if (w) {
        auto page = qobject_cast<SieveEditorPageWidget *>(w);
        if (page) {
            page->upperCase();
        }
    }
}

void SieveEditorMainWidget::slotSentenceCase()
{
    QWidget *w = mTabWidget->currentWidget();
    if (w) {
        auto page = qobject_cast<SieveEditorPageWidget *>(w);
        if (page) {
            page->sentenceCase();
        }
    }
}

void SieveEditorMainWidget::slotLowerCase()
{
    QWidget *w = mTabWidget->currentWidget();
    if (w) {
        auto page = qobject_cast<SieveEditorPageWidget *>(w);
        if (page) {
            page->lowerCase();
        }
    }
}

void SieveEditorMainWidget::slotScriptModified(bool modified, SieveEditorPageWidget *page)
{
    const int index = mTabWidget->indexOf(page);
    if (index >= 0) {
        if (!mScriptColor.isValid()) {
            updatePaletteColor();
        }
        mTabWidget->tabBar()->setTabTextColor(index, modified ? mModifiedScriptColor : mScriptColor);
    }
}

void SieveEditorMainWidget::slotGeneralPaletteChanged()
{
    updatePaletteColor();
    for (int i = mTabWidget->count() - 1; i >= 0; --i) {
        auto page = qobject_cast<SieveEditorPageWidget *>(mTabWidget->widget(i));
        if (page) {
            mTabWidget->tabBar()->setTabTextColor(i, page->isModified() ? mModifiedScriptColor : mScriptColor);
        }
    }
}

void SieveEditorMainWidget::slotZoomIn()
{
    QWidget *w = mTabWidget->currentWidget();
    if (w) {
        auto page = qobject_cast<SieveEditorPageWidget *>(w);
        if (page) {
            page->zoomIn();
        }
    }
}

void SieveEditorMainWidget::slotZoomOut()
{
    QWidget *w = mTabWidget->currentWidget();
    if (w) {
        auto page = qobject_cast<SieveEditorPageWidget *>(w);
        if (page) {
            page->zoomOut();
        }
    }
}

void SieveEditorMainWidget::slotZoomReset()
{
    QWidget *w = mTabWidget->currentWidget();
    if (w) {
        auto page = qobject_cast<SieveEditorPageWidget *>(w);
        if (page) {
            page->zoomReset();
        }
    }
}

void SieveEditorMainWidget::slotWordWrap(bool state)
{
    QWidget *w = mTabWidget->currentWidget();
    if (w) {
        auto page = qobject_cast<SieveEditorPageWidget *>(w);
        if (page) {
            page->setWordWrap(state);
        }
    }
}

void SieveEditorMainWidget::slotPrintPreview()
{
    QWidget *w = mTabWidget->currentWidget();
    if (w) {
        auto page = qobject_cast<SieveEditorPageWidget *>(w);
        if (page) {
            page->printPreview();
        }
    }
}

void SieveEditorMainWidget::slotPrint()
{
    QWidget *w = mTabWidget->currentWidget();
    if (w) {
        auto page = qobject_cast<SieveEditorPageWidget *>(w);
        if (page) {
            page->print();
        }
    }
}

void SieveEditorMainWidget::updatePaletteColor()
{
    const KColorScheme scheme(QPalette::Active, KColorScheme::View);
    mScriptColor = scheme.foreground(KColorScheme::NormalText).color();

    mModifiedScriptColor = scheme.foreground(KColorScheme::NegativeText).color();
}

void SieveEditorMainWidget::forceCloseTab(int index)
{
    auto page = qobject_cast<SieveEditorPageWidget *>(mTabWidget->widget(index));
    if (page) {
        mTabWidget->removeTab(index);
        delete page;
        updateStackedWidget();
    }
}

void SieveEditorMainWidget::slotTabCloseRequested(int index)
{
    auto page = qobject_cast<SieveEditorPageWidget *>(mTabWidget->widget(index));
    if (page) {
        if (page->isModified()) {
            const int result = KMessageBox::questionTwoActionsCancel(this,
                                                                     i18n("Script was modified. Do you want to save before closing?"),
                                                                     i18nc("@title:window", "Close script"),
                                                                     KStandardGuiItem::save(),
                                                                     KStandardGuiItem::close(),
                                                                     KStandardGuiItem::cancel());
            if (result == KMessageBox::ButtonCode::PrimaryAction) {
                if (page->uploadScriptAndCloseTab(index)) {
                    return;
                }
            } else if (result == KMessageBox::Cancel) {
                return;
            }
        }
        mTabWidget->removeTab(index);
        delete page;
    }
    updateStackedWidget();
}

void SieveEditorMainWidget::slotTabCloseAllRequested()
{
    for (int i = mTabWidget->count() - 1; i >= 0; --i) {
        slotTabCloseRequested(i);
    }
}

void SieveEditorMainWidget::slotTabRemoveAllExclude(int index)
{
    for (int i = mTabWidget->count() - 1; i >= 0; --i) {
        if (i == index) {
            continue;
        }
        slotTabCloseRequested(i);
    }
}

KSieveUi::SieveEditorWidget::EditorMode SieveEditorMainWidget::pageMode() const
{
    QWidget *w = mTabWidget->currentWidget();
    if (w) {
        auto page = qobject_cast<SieveEditorPageWidget *>(w);
        if (page) {
            return page->pageMode();
        }
    }
    return KSieveUi::SieveEditorWidget::Unknown;
}

void SieveEditorMainWidget::openBookmarkUrl(const QUrl &url)
{
    QWidget *w = mTabWidget->currentWidget();
    if (w) {
        auto page = qobject_cast<SieveEditorPageWidget *>(w);
        if (page) {
            page->openBookmarkUrl(url);
        }
    }
}

QString SieveEditorMainWidget::currentHelpTitle() const
{
    QWidget *w = mTabWidget->currentWidget();
    if (w) {
        auto page = qobject_cast<SieveEditorPageWidget *>(w);
        if (page) {
            return page->currentHelpTitle();
        }
    }
    return {};
}

QUrl SieveEditorMainWidget::currentHelpUrl() const
{
    QWidget *w = mTabWidget->currentWidget();
    if (w) {
        auto page = qobject_cast<SieveEditorPageWidget *>(w);
        if (page) {
            return page->currentHelpUrl();
        }
    }
    return {};
}

bool SieveEditorMainWidget::printSupportEnabled() const
{
    QWidget *w = mTabWidget->currentWidget();
    if (w) {
        auto page = qobject_cast<SieveEditorPageWidget *>(w);
        if (page) {
            return page->printSupportEnabled();
        }
    }
    return false;
}

bool SieveEditorMainWidget::isTextEditor() const
{
    QWidget *w = mTabWidget->currentWidget();
    if (w) {
        auto page = qobject_cast<SieveEditorPageWidget *>(w);
        if (page) {
            return page->isTextEditor();
        }
    }
    return false;
}

bool SieveEditorMainWidget::event(QEvent *e)
{
    if (e->type() == QEvent::ApplicationPaletteChange) {
        slotGeneralPaletteChanged();
    }
    return QSplitter::event(e);
}

#include "moc_sieveeditormainwidget.cpp"
