/*
   SPDX-FileCopyrightText: 2016-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "importimapsettingfinishpage.h"
#include <QVBoxLayout>
#include <KLocalizedString>
#include <QTextEdit>
#include <QLabel>

ImportImapSettingFinishPage::ImportImapSettingFinishPage(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *mainlayout = new QVBoxLayout(this);
    mainlayout->setObjectName(QStringLiteral("mainlayout"));
    mainlayout->setContentsMargins(0, 0, 0, 0);

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
