/*
   SPDX-FileCopyrightText: 2016-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "importimapsettingnofoundpage.h"
#include <QVBoxLayout>
#include <QLabel>
#include <KLocalizedString>

ImportImapSettingNoFoundPage::ImportImapSettingNoFoundPage(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainlayout"));
    mainLayout->setContentsMargins(0, 0, 0, 0);

    QLabel *lab = new QLabel(i18n("No settings found to import."));
    QFont font = lab->font();
    font.setBold(true);
    font.setPointSizeF(font.pointSizeF() * 1.5);
    lab->setFont(font);
    lab->setAlignment(Qt::AlignCenter);
    lab->setObjectName(QStringLiteral("label"));
    mainLayout->addWidget(lab);
}

ImportImapSettingNoFoundPage::~ImportImapSettingNoFoundPage()
{
}
