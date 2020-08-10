/*
   SPDX-FileCopyrightText: 2014-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef SieveEditorConfigureServerPage_H
#define SieveEditorConfigureServerPage_H

#include <QWidget>
#include "libsieveeditor_private_export.h"

class LIBSIEVEEDITOR_TESTS_EXPORT SieveEditorConfigureServerPage : public QWidget
{
    Q_OBJECT
public:
    explicit SieveEditorConfigureServerPage(QWidget *parent = nullptr);
    ~SieveEditorConfigureServerPage();

Q_SIGNALS:
    void configureClicked();
    void importSieveSettings();
};

#endif // SieveEditorConfigureServerPage_H
