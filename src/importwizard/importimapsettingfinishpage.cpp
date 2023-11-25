/*
   SPDX-FileCopyrightText: 2016-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "importimapsettingfinishpage.h"
#include <KLocalizedString>
#include <QLabel>
#include <QTextEdit>
#include <QVBoxLayout>

ImportImapSettingFinishPage::ImportImapSettingFinishPage(QWidget *parent)
    : QWidget(parent)
    , mTextEdit(new QTextEdit(this))
{
    auto mainlayout = new QVBoxLayout(this);
    mainlayout->setObjectName(QLatin1StringView("mainlayout"));

    auto label = new QLabel(i18n("IMAP settings were imported:"), this);
    label->setObjectName(QLatin1StringView("label"));
    mainlayout->addWidget(label);

    mTextEdit->setObjectName(QLatin1StringView("textedit"));
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
