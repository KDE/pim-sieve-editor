/*
   SPDX-FileCopyrightText: 2016-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "importimapsettingsearchpage.h"
using namespace Qt::Literals::StringLiterals;

#include <KLocalizedString>
#include <QLabel>
#include <QListWidget>
#include <QVBoxLayout>

ImportImapSettingSearchPage::ImportImapSettingSearchPage(QWidget *parent)
    : QWidget(parent)
    , mFoundProgramList(new QListWidget(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName("mainlayout"_L1);
    auto label = new QLabel(i18nc("@label:textbox", "Please select the program from which you like to import IMAP settings:"), this);
    label->setObjectName("label"_L1);
    mainLayout->addWidget(label);

    mFoundProgramList->setObjectName("foundprogramlist"_L1);
    mainLayout->addWidget(mFoundProgramList);
    connect(mFoundProgramList, &QListWidget::itemChanged, this, &ImportImapSettingSearchPage::slotItemChanged);
}

ImportImapSettingSearchPage::~ImportImapSettingSearchPage() = default;

void ImportImapSettingSearchPage::setProgramList(const QStringList &lst)
{
    for (const QString &prog : lst) {
        auto item = new QListWidgetItem(prog, mFoundProgramList);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Unchecked);
    }
}

QStringList ImportImapSettingSearchPage::selectedPrograms() const
{
    QStringList checkedItems;
    const int nbProgram = {mFoundProgramList->count()};
    for (int i = 0; i < nbProgram; ++i) {
        QListWidgetItem *item = mFoundProgramList->item(i);
        if (item->checkState() == Qt::Checked) {
            checkedItems << item->text();
        }
    }
    return checkedItems;
}

void ImportImapSettingSearchPage::slotItemChanged()
{
    bool hasSelectedItem = false;
    const int nbProgram = {mFoundProgramList->count()};
    for (int i = 0; i < nbProgram; ++i) {
        QListWidgetItem *item = mFoundProgramList->item(i);
        if (item->checkState() == Qt::Checked) {
            hasSelectedItem = true;
            break;
        }
    }
    Q_EMIT needToImportSettings(hasSelectedItem);
}

#include "moc_importimapsettingsearchpage.cpp"
