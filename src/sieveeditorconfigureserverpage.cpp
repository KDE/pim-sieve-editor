/*
   SPDX-FileCopyrightText: 2014-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "sieveeditorconfigureserverpage.h"

#include <KLocalizedString>

#include <QHBoxLayout>
#include <QPushButton>

SieveEditorConfigureServerPage::SieveEditorConfigureServerPage(QWidget *parent)
    : QWidget(parent)
{
    auto hbox = new QHBoxLayout(this);
    hbox->addStretch(1);
    auto button = new QPushButton(i18n("Configure..."));
    button->setObjectName(QStringLiteral("configure_button"));
    button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(button, &QAbstractButton::clicked, this, &SieveEditorConfigureServerPage::configureClicked);

    auto importSieveSettingsButton = new QPushButton(i18n("Import Sieve Settings..."));
    importSieveSettingsButton->setObjectName(QStringLiteral("import_sieve_settings_button"));
    importSieveSettingsButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(importSieveSettingsButton, &QAbstractButton::clicked, this, &SieveEditorConfigureServerPage::importSieveSettings);

    hbox->addWidget(button);
    hbox->addWidget(importSieveSettingsButton);
    hbox->addStretch(1);
}

SieveEditorConfigureServerPage::~SieveEditorConfigureServerPage() = default;

#include "moc_sieveeditorconfigureserverpage.cpp"
