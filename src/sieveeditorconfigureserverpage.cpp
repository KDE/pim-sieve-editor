/*
   SPDX-FileCopyrightText: 2014-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "sieveeditorconfigureserverpage.h"
using namespace Qt::Literals::StringLiterals;

#include <KLocalizedString>

#include <QHBoxLayout>
#include <QPushButton>

SieveEditorConfigureServerPage::SieveEditorConfigureServerPage(QWidget *parent)
    : QWidget(parent)
{
    auto hbox = new QHBoxLayout(this);
    hbox->addStretch(1);
    auto button = new QPushButton(i18nc("@action:button", "Configure…"), this);
    button->setObjectName("configure_button"_L1);
    button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(button, &QAbstractButton::clicked, this, &SieveEditorConfigureServerPage::configureClicked);

    auto importSieveSettingsButton = new QPushButton(QIcon::fromTheme("document-import"_L1), i18nc("@action:button", "Import Sieve Settings…"));
    importSieveSettingsButton->setObjectName("import_sieve_settings_button"_L1);
    importSieveSettingsButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(importSieveSettingsButton, &QAbstractButton::clicked, this, &SieveEditorConfigureServerPage::importSieveSettings);

    hbox->addWidget(button);
    hbox->addWidget(importSieveSettingsButton);
    hbox->addStretch(1);
}

SieveEditorConfigureServerPage::~SieveEditorConfigureServerPage() = default;

#include "moc_sieveeditorconfigureserverpage.cpp"
