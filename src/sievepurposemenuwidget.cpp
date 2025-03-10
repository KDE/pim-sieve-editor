/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "sievepurposemenuwidget.h"
#include "sieveeditormainwidget.h"
#include <KSieveUi/SieveEditorWidget>

SievePurposeMenuWidget::SievePurposeMenuWidget(QWidget *parentWidget, QObject *parent)
    : PimCommon::PurposeMenuWidget(parentWidget, parent)
{
}

SievePurposeMenuWidget::~SievePurposeMenuWidget() = default;

QByteArray SievePurposeMenuWidget::text()
{
    if (mSieveEditorWidget) {
        return mSieveEditorWidget->currentText().toUtf8();
    }
    return {};
}

void SievePurposeMenuWidget::setEditorWidget(SieveEditorMainWidget *editor)
{
    mSieveEditorWidget = editor;
}

#include "moc_sievepurposemenuwidget.cpp"
