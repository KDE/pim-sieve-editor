/*
   SPDX-FileCopyrightText: 2015-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <KBookmarkManager>
#include <KBookmarkOwner>

#include <QObject>

class KBookmarkMenu;
class QMenu;
class KActionCollection;
class SieveEditorMainWindow;
class SieveEditorBookmarks : public QObject, public KBookmarkOwner
{
    Q_OBJECT
public:
    explicit SieveEditorBookmarks(SieveEditorMainWindow *mainWindow, KActionCollection *collection, QMenu *menu, QObject *parent = nullptr);
    ~SieveEditorBookmarks() override;

    QMenu *menu() const;

    void openBookmark(const KBookmark &bm, Qt::MouseButtons mb, Qt::KeyboardModifiers km) override;

    [[nodiscard]] QString currentTitle() const override;
    [[nodiscard]] QUrl currentUrl() const override;

Q_SIGNALS:
    void openUrl(const QUrl &url);

private:
    Q_DISABLE_COPY(SieveEditorBookmarks)
    KBookmarkManager *m_bookmarkManager = nullptr;
    KBookmarkMenu *mBookmarkMenu = nullptr;
    QMenu *const mMenu;
    SieveEditorMainWindow *const mMainWindow;
};
