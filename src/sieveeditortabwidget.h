/*
   SPDX-FileCopyrightText: 2014-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef SIEVEEDITORTABWIDGET_H
#define SIEVEEDITORTABWIDGET_H
#include <QTabWidget>
class KActionCollection;
class SieveEditorTabWidget : public QTabWidget
{
    Q_OBJECT
public:
    explicit SieveEditorTabWidget(KActionCollection *ac, QWidget *parent = nullptr);
    ~SieveEditorTabWidget();

Q_SIGNALS:
    void tabRemoveAllExclude(int index);
    void tabCloseRequestedIndex(int index);
    void tabCloseAllTab();

private:
    void slotTabContextMenuRequest(const QPoint &pos);
    void slotCloseCurrentTab();

private:
    void initActions(KActionCollection *ac);
};

#endif // SIEVEEDITORTABWIDGET_H
