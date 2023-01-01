/*
   SPDX-FileCopyrightText: 2013-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QWidget>

namespace Ui
{
class SieveEditorConfigureServerWidget;
}

class SieveEditorConfigureServerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SieveEditorConfigureServerWidget(QWidget *parent = nullptr);
    ~SieveEditorConfigureServerWidget() override;

    void readConfig();
    void writeConfig();

private:
    void slotModifyServer();
    void slotAddServer();
    void slotDeleteServer();
    void slotItemSelectionChanged();

private:
    Ui::SieveEditorConfigureServerWidget *const ui;
};
