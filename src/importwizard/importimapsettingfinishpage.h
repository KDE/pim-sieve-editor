/*
   SPDX-FileCopyrightText: 2016-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef IMPORTIMAPSETTINGFINISHPAGE_H
#define IMPORTIMAPSETTINGFINISHPAGE_H
#include "libsieveeditor_private_export.h"
#include <QWidget>
class QTextEdit;
class LIBSIEVEEDITOR_TESTS_EXPORT ImportImapSettingFinishPage : public QWidget
{
    Q_OBJECT
public:
    explicit ImportImapSettingFinishPage(QWidget *parent = nullptr);
    ~ImportImapSettingFinishPage();

    void addSummary(const QString &name);

private:
    QTextEdit *mTextEdit = nullptr;
};

#endif // IMPORTIMAPSETTINGFINISHPAGE_H
