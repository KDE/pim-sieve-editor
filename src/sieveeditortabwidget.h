/*
   SPDX-FileCopyrightText: 2014-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once
#include <QTabWidget>
class KActionCollection;
class SieveEditorTabWidget : public QTabWidget
{
    Q_OBJECT
public:
    explicit SieveEditorTabWidget(KActionCollection *ac, QWidget *parent = nullptr);
    ~SieveEditorTabWidget() override;

Q_SIGNALS:
    void tabRemoveAllExclude(int index);
    void tabCloseRequestedIndex(int index);
    void tabCloseAllTab();

private:
    void slotTabContextMenuRequest(const QPoint &pos);
    void slotCloseCurrentTab();
    void initActions(KActionCollection *ac);
};
