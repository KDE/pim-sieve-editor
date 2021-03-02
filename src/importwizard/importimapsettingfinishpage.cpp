/*
   SPDX-FileCopyrightText: 2016-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "importimapsettingfinishpage.h"
#include <KLocalizedString>
#include <QLabel>
#include <QTextEdit>
#include <QVBoxLayout>

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
