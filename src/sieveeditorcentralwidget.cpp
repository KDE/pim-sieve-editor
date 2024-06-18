/*
   SPDX-FileCopyrightText: 2014-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "sieveeditorcentralwidget.h"
#include "sieveeditorconfigureserverpage.h"
#include "sieveeditormainwidget.h"
#include <PimCommon/PurposeMenuMessageWidget>
#include <QVBoxLayout>

SieveEditorCentralWidget::SieveEditorCentralWidget(QWidget *parent, KActionCollection *ac)
    : QStackedWidget(parent)
    , mConfigureWidget(new SieveEditorConfigureServerPage(this))
    , mSieveEditorMainWidget(new SieveEditorMainWidget(ac, this))
    , mPurposeMenuMessageWidget(new PimCommon::PurposeMenuMessageWidget(this))
    , mMainWidget(new QWidget)
{
    connect(mConfigureWidget, &SieveEditorConfigureServerPage::configureClicked, this, &SieveEditorCentralWidget::configureClicked);
    connect(mConfigureWidget, &SieveEditorConfigureServerPage::importSieveSettings, this, &SieveEditorCentralWidget::importSieveSettings);
    addWidget(mConfigureWidget);

    auto mainWidgetLayout = new QVBoxLayout(mMainWidget);
    mainWidgetLayout->setContentsMargins({});
    mainWidgetLayout->setSpacing(0);

    mPurposeMenuMessageWidget->setPosition(KMessageWidget::Header);
    mainWidgetLayout->addWidget(mPurposeMenuMessageWidget);
    mainWidgetLayout->addWidget(mSieveEditorMainWidget);
    connect(mSieveEditorMainWidget, &SieveEditorMainWidget::serverSieveFound, this, &SieveEditorCentralWidget::slotServerSieveFound);
    addWidget(mMainWidget);
    setCurrentWidget(mConfigureWidget);
}

SieveEditorCentralWidget::~SieveEditorCentralWidget() = default;

void SieveEditorCentralWidget::slotServerSieveFound(bool hasServer)
{
    if (hasServer) {
        setCurrentWidget(mMainWidget);
    } else {
        setCurrentWidget(mConfigureWidget);
    }
}

SieveEditorMainWidget *SieveEditorCentralWidget::sieveEditorMainWidget() const
{
    return mSieveEditorMainWidget;
}

void SieveEditorCentralWidget::slotShareError(const QString &message)
{
    mPurposeMenuMessageWidget->slotShareError(message);
}

void SieveEditorCentralWidget::slotShareSuccess(const QString &message)
{
    mPurposeMenuMessageWidget->slotShareSuccess(message);
}

#include "moc_sieveeditorcentralwidget.cpp"
