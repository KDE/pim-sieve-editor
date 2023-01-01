/*
   SPDX-FileCopyrightText: 2014-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "sieveeditortabwidget.h"

#include <KActionCollection>
#include <KLocalizedString>
#include <QIcon>
#include <QMenu>

#include <QAction>
#include <QTabBar>

SieveEditorTabWidget::SieveEditorTabWidget(KActionCollection *ac, QWidget *parent)
    : QTabWidget(parent)
{
    initActions(ac);
    setMovable(true);
    setTabsClosable(true);
    connect(this, &QTabWidget::tabCloseRequested, this, &SieveEditorTabWidget::tabCloseRequestedIndex);
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &SieveEditorTabWidget::customContextMenuRequested, this, &SieveEditorTabWidget::slotTabContextMenuRequest);
}

SieveEditorTabWidget::~SieveEditorTabWidget() = default;

void SieveEditorTabWidget::slotTabContextMenuRequest(const QPoint &pos)
{
    QTabBar *bar = tabBar();
    if (count() < 1) {
        return;
    }

    const int indexBar = bar->tabAt(bar->mapFrom(this, pos));
    if (indexBar == -1) {
        return;
    }

    QMenu menu(this);
    QAction *closeTab = menu.addAction(i18nc("@action:inmenu", "Close Tab"));
    closeTab->setIcon(QIcon::fromTheme(QStringLiteral("tab-close")));

    const bool hasSeveralTabs = (count() > 1);
    QAction *allOther = menu.addAction(i18nc("@action:inmenu", "Close All Other Tabs"));
    allOther->setEnabled(hasSeveralTabs);
    allOther->setIcon(QIcon::fromTheme(QStringLiteral("tab-close-other")));

    QAction *allTab = menu.addAction(i18nc("@action:inmenu", "Close All Tabs"));
    allTab->setEnabled(hasSeveralTabs);
    allTab->setIcon(QIcon::fromTheme(QStringLiteral("tab-close")));

    QAction *action = menu.exec(mapToGlobal(pos));

    if (action == allOther) { // Close all other tabs
        Q_EMIT tabRemoveAllExclude(indexBar);
    } else if (action == closeTab) {
        Q_EMIT tabCloseRequestedIndex(indexBar);
    } else if (action == allTab) {
        Q_EMIT tabCloseAllTab();
    }
}

void SieveEditorTabWidget::initActions(KActionCollection *ac)
{
    if (ac) {
        auto closeCurrentTabAct = new QAction(i18nc("@action:inmenu", "Close Tab"), this);
        ac->addAction(QStringLiteral("close_current_tab"), closeCurrentTabAct);
        ac->setDefaultShortcut(closeCurrentTabAct, QKeySequence(Qt::CTRL | Qt::Key_W));
        closeCurrentTabAct->setIcon(QIcon::fromTheme(QStringLiteral("tab-close")));
        connect(closeCurrentTabAct, &QAction::triggered, this, &SieveEditorTabWidget::slotCloseCurrentTab);
    }
}

void SieveEditorTabWidget::slotCloseCurrentTab()
{
    const int curIndex = currentIndex();
    if (curIndex != -1) {
        Q_EMIT tabCloseRequestedIndex(curIndex);
    }
}
