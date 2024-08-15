/*
   SPDX-FileCopyrightText: 2016-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "libsieveeditor_private_export.h"
#include <QWidget>

class LIBSIEVEEDITOR_TESTS_EXPORT ImportImapSettingNoFoundPage : public QWidget
{
    Q_OBJECT
public:
    explicit ImportImapSettingNoFoundPage(QWidget *parent = nullptr);
    ~ImportImapSettingNoFoundPage() override;
};
