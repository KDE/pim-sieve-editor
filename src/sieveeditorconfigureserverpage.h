/*
   SPDX-FileCopyrightText: 2014-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "libsieveeditor_private_export.h"
#include <QWidget>

class LIBSIEVEEDITOR_TESTS_EXPORT SieveEditorConfigureServerPage : public QWidget
{
    Q_OBJECT
public:
    explicit SieveEditorConfigureServerPage(QWidget *parent = nullptr);
    ~SieveEditorConfigureServerPage() override;

Q_SIGNALS:
    void configureClicked();
    void importSieveSettings();
};
