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

#include "importimapsettingsearchpage.h"

#include <QLabel>
#include <QListWidget>
#include <QVBoxLayout>
#include <KLocalizedString>

ImportImapSettingSearchPage::ImportImapSettingSearchPage(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QLabel *label = new QLabel(i18n("Please select the program from which you like to import IMAP settings:"), this);
    label->setObjectName(QStringLiteral("label"));
    mainLayout->addWidget(label);

    mFoundProgramList = new QListWidget(this);
    mFoundProgramList->setObjectName(QStringLiteral("foundprogramlist"));
    mainLayout->addWidget(mFoundProgramList);
}

ImportImapSettingSearchPage::~ImportImapSettingSearchPage()
{

}

void ImportImapSettingSearchPage::setProgramList(const QStringList &lst)
{
    for (const QString &prog : lst) {
        QListWidgetItem *item = new QListWidgetItem(prog, mFoundProgramList);
        item->setFlags(item->flags() & ~ Qt::ItemIsUserCheckable);
    }
}

QStringList ImportImapSettingSearchPage::selectedPrograms() const
{
    QStringList checkedItems;
    for (int i = 0; i < mFoundProgramList->count(); ++i) {
        QListWidgetItem *item = mFoundProgramList->item(i);
        if (item->checkState() == Qt::Checked) {
            checkedItems << item->text();
        }
    }
    return checkedItems;
}
