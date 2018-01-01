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

#include "sieveeditorconfigureserverpage.h"

#include <KLocalizedString>

#include <QHBoxLayout>
#include <QPushButton>

SieveEditorConfigureServerPage::SieveEditorConfigureServerPage(QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout *hbox = new QHBoxLayout(this);
    hbox->addStretch(1);
    QPushButton *button = new QPushButton(i18n("Configure..."));
    button->setObjectName(QStringLiteral("configure_button"));
    button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(button, &QAbstractButton::clicked, this, &SieveEditorConfigureServerPage::configureClicked);

    QPushButton *importSieveSettings = new QPushButton(i18n("Import Sieve Settings..."));
    importSieveSettings->setObjectName(QStringLiteral("import_sieve_settings_button"));
    importSieveSettings->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(importSieveSettings, &QAbstractButton::clicked, this, &SieveEditorConfigureServerPage::importSieveSettings);

    hbox->addWidget(button);
    hbox->addWidget(importSieveSettings);
    hbox->addStretch(1);
}

SieveEditorConfigureServerPage::~SieveEditorConfigureServerPage()
{
}
