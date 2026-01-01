/*
   SPDX-FileCopyrightText: 2016-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once
#include "libsieveeditor_private_export.h"
#include <QWidget>
class QTextEdit;
class LIBSIEVEEDITOR_TESTS_EXPORT ImportImapSettingFinishPage : public QWidget
{
    Q_OBJECT
public:
    explicit ImportImapSettingFinishPage(QWidget *parent = nullptr);
    ~ImportImapSettingFinishPage() override;

    void addSummary(const QString &name);

private:
    QTextEdit *const mTextEdit;
};
