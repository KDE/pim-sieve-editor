/*
   SPDX-FileCopyrightText: 2015-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "sieveeditorbookmarks.h"
#include "sieveeditormainwindow.h"
#include <KActionCollection>
#include <KBookmarkManager>
#include <KBookmarkMenu>
#include <QDir>
#include <QMenu>
#include <QStandardPaths>

SieveEditorBookmarks::SieveEditorBookmarks(SieveEditorMainWindow *mainWindow, KActionCollection *collection, QMenu *menu, QObject *parent)
    : QObject(parent)
    , KBookmarkOwner()
    , mMenu(menu)
    , mMainWindow(mainWindow)
{
    QString bookmarkFile = QStandardPaths::locate(QStandardPaths::GenericDataLocation, QStringLiteral("sieveeditor/bookmarks.xml"));

    if (bookmarkFile.isEmpty()) {
        bookmarkFile = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + QStringLiteral("/sieveeditor");
        QDir().mkpath(bookmarkFile);
        bookmarkFile += QStringLiteral("/bookmarks.xml");
    }

    KBookmarkManager *manager = KBookmarkManager::managerForFile(bookmarkFile, QStringLiteral("sieveeditor"));
    manager->setUpdate(true);

    mBookmarkMenu = new KBookmarkMenu(manager, this, mMenu);
    collection->addActions(mMenu->actions());
}

SieveEditorBookmarks::~SieveEditorBookmarks()
{
    delete mBookmarkMenu;
}

QMenu *SieveEditorBookmarks::menu() const
{
    return mMenu;
}

void SieveEditorBookmarks::openBookmark(const KBookmark &bm, Qt::MouseButtons mb, Qt::KeyboardModifiers km)
{
    Q_UNUSED(mb)
    Q_UNUSED(km)
    Q_EMIT openUrl(bm.url());
}

QString SieveEditorBookmarks::currentTitle() const
{
    return mMainWindow->currentHelpTitle();
}

QUrl SieveEditorBookmarks::currentUrl() const
{
    return mMainWindow->currentHelpUrl();
}
