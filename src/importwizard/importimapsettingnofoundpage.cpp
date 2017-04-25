/*
   Copyright (C) 2016-2017 Montel Laurent <montel@kde.org>

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

#include "importimapsettingnofoundpage.h"
#include <QVBoxLayout>
#include <QLabel>
#include <KLocalizedString>

ImportImapSettingNoFoundPage::ImportImapSettingNoFoundPage(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainlayout"));
    mainLayout->setMargin(0);

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
