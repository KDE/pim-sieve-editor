/*
   SPDX-FileCopyrightText: 2016-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "importimapsettingfinishpage.h"
using namespace Qt::Literals::StringLiterals;

#include <KLocalizedString>
#include <QLabel>
#include <QTextEdit>
#include <QVBoxLayout>

ImportImapSettingFinishPage::ImportImapSettingFinishPage(QWidget *parent)
    : QWidget(parent)
    , mTextEdit(new QTextEdit(this))
{
    auto mainlayout = new QVBoxLayout(this);
    mainlayout->setObjectName("mainlayout"_L1);

    auto label = new QLabel(i18n("IMAP settings were imported:"), this);
    label->setObjectName("label"_L1);
    mainlayout->addWidget(label);

    mTextEdit->setObjectName("textedit"_L1);
    mTextEdit->setAcceptRichText(true);
    mTextEdit->setReadOnly(true);
    mainlayout->addWidget(mTextEdit);
}

ImportImapSettingFinishPage::~ImportImapSettingFinishPage() = default;

void ImportImapSettingFinishPage::addSummary(const QString &name)
{
    mTextEdit->append(name);
}

#include "moc_importimapsettingfinishpage.cpp"
