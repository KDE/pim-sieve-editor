/*
   SPDX-FileCopyrightText: 2013-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef SIEVEEDITORMANAGESIEVEWIDGET_H
#define SIEVEEDITORMANAGESIEVEWIDGET_H

#include <KSieveUi/ManageSieveWidget>

class SieveEditorManageSieveWidget : public KSieveUi::ManageSieveWidget
{
    Q_OBJECT
public:
    explicit SieveEditorManageSieveWidget(QWidget *parent = nullptr);
    ~SieveEditorManageSieveWidget() override;

protected:
    Q_REQUIRED_RESULT bool refreshList() override;
};

#endif // SIEVEEDITORMANAGESIEVEWIDGET_H
