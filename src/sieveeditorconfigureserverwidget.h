/*
   SPDX-FileCopyrightText: 2013-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef SIEVEEDITORCONFIGURESERVERWIDGET_H
#define SIEVEEDITORCONFIGURESERVERWIDGET_H

#include <QWidget>

namespace Ui {
class SieveEditorConfigureServerWidget;
}

class SieveEditorConfigureServerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SieveEditorConfigureServerWidget(QWidget *parent = nullptr);
    ~SieveEditorConfigureServerWidget();

    void readConfig();
    void writeConfig();

private:
    void slotModifyServer();
    void slotAddServer();
    void slotDeleteServer();
    void slotItemSelectionChanged();

private:
    Ui::SieveEditorConfigureServerWidget *ui = nullptr;
};

#endif // SIEVEEDITORCONFIGURESERVERWIDGET_H
