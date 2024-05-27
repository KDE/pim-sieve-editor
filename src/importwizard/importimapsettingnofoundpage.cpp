/*
   SPDX-FileCopyrightText: 2016-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "importimapsettingnofoundpage.h"
using namespace Qt::Literals::StringLiterals;

#include <KLocalizedString>
#include <QLabel>
#include <QVBoxLayout>

ImportImapSettingNoFoundPage::ImportImapSettingNoFoundPage(QWidget *parent)
    : QWidget(parent)
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName("mainlayout"_L1);

    auto lab = new QLabel(i18nc("@label:textbox", "No settings found to import."));
    QFont font = lab->font();
    font.setBold(true);
    font.setPointSizeF(font.pointSizeF() * 1.5);
    lab->setFont(font);
    lab->setAlignment(Qt::AlignCenter);
    lab->setObjectName("label"_L1);
    mainLayout->addWidget(lab);
}

ImportImapSettingNoFoundPage::~ImportImapSettingNoFoundPage() = default;

#include "moc_importimapsettingnofoundpage.cpp"
