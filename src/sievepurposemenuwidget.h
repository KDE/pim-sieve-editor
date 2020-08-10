/*
   SPDX-FileCopyrightText: 2018-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef SIEVEPURPOSEMENUWIDGET_H
#define SIEVEPURPOSEMENUWIDGET_H

#include <PimCommon/PurposeMenuWidget>
class SieveEditorMainWidget;
class SievePurposeMenuWidget : public PimCommon::PurposeMenuWidget
{
    Q_OBJECT
public:
    explicit SievePurposeMenuWidget(QWidget *parentWidget, QObject *parent = nullptr);
    ~SievePurposeMenuWidget() override;

    Q_REQUIRED_RESULT QByteArray text() override;
    void setEditorWidget(SieveEditorMainWidget *editor);
private:
    SieveEditorMainWidget *mSieveEditorWidget = nullptr;
};

#endif // SIEVEPURPOSEMENUWIDGET_H
