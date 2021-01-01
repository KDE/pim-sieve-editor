/*
   Copyright (C) 2016-2021 Laurent Montel <montel@kde.org>

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

#include "importimapsettingfinishpage.h"
#include <QVBoxLayout>
#include <KLocalizedString>
#include <QTextEdit>
#include <QLabel>

ImportImapSettingFinishPage::ImportImapSettingFinishPage(QWidget *parent)
    : QWidget(parent)
{
    auto mainlayout = new QVBoxLayout(this);
    mainlayout->setObjectName(QStringLiteral("mainlayout"));

    QLabel *label = new QLabel(i18n("IMAP settings were imported:"), this);
    label->setObjectName(QStringLiteral("label"));
    mainlayout->addWidget(label);

    mTextEdit = new QTextEdit(this);
    mTextEdit->setObjectName(QStringLiteral("textedit"));
    mTextEdit->setAcceptRichText(true);
    mTextEdit->setReadOnly(true);
    mainlayout->addWidget(mTextEdit);
}

ImportImapSettingFinishPage::~ImportImapSettingFinishPage()
{
}

void ImportImapSettingFinishPage::addSummary(const QString &name)
{
    mTextEdit->append(name);
}
