/*
   Copyright (C) 2014-2018 Montel Laurent <montel@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
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
