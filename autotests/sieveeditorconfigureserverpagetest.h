/*
   SPDX-FileCopyrightText: 2015-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class SieveEditorConfigureServerPageTest : public QObject
{
    Q_OBJECT
public:
    explicit SieveEditorConfigureServerPageTest(QObject *parent = nullptr);
    ~SieveEditorConfigureServerPageTest() override;

private Q_SLOTS:
    void shouldEmitSignalWhenClickOnConfigureButton();
    void shouldEmitSignalWhenClickOnImportSieveButton();
    void shouldHaveDefaultValue();
};
