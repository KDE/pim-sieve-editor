/*
   SPDX-FileCopyrightText: 2016-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef IMPORTIMAPSETTINGNOFOUNDPAGE_H
#define IMPORTIMAPSETTINGNOFOUNDPAGE_H

#include <QWidget>
#include "libsieveeditor_private_export.h"

class LIBSIEVEEDITOR_TESTS_EXPORT ImportImapSettingNoFoundPage : public QWidget
{
    Q_OBJECT
public:
    explicit ImportImapSettingNoFoundPage(QWidget *parent = nullptr);
    ~ImportImapSettingNoFoundPage();
};

#endif // IMPORTIMAPSETTINGNOFOUNDPAGE_H
