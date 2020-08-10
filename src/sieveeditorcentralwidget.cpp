/*
   SPDX-FileCopyrightText: 2014-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "sieveeditorcentralwidget.h"
#include "sieveeditormainwidget.h"
#include "sieveeditorconfigureserverpage.h"

SieveEditorCentralWidget::SieveEditorCentralWidget(QWidget *parent, KActionCollection *ac)
    : QStackedWidget(parent)
{
    mConfigureWidget = new SieveEditorConfigureServerPage;
    connect(mConfigureWidget, &SieveEditorConfigureServerPage::configureClicked, this, &SieveEditorCentralWidget::configureClicked);
    connect(mConfigureWidget, &SieveEditorConfigureServerPage::importSieveSettings, this, &SieveEditorCentralWidget::importSieveSettings);
    addWidget(mConfigureWidget);
    mSieveEditorMainWidget = new SieveEditorMainWidget(ac);
    connect(mSieveEditorMainWidget, &SieveEditorMainWidget::serverSieveFound, this, &SieveEditorCentralWidget::slotServerSieveFound);
    addWidget(mSieveEditorMainWidget);
    setCurrentWidget(mConfigureWidget);
}

SieveEditorCentralWidget::~SieveEditorCentralWidget()
{
}

void SieveEditorCentralWidget::slotServerSieveFound(bool hasServer)
{
    if (hasServer) {
        setCurrentWidget(mSieveEditorMainWidget);
    } else {
        setCurrentWidget(mConfigureWidget);
    }
}

SieveEditorMainWidget *SieveEditorCentralWidget::sieveEditorMainWidget() const
{
    return mSieveEditorMainWidget;
}
