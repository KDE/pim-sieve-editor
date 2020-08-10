/*
   SPDX-FileCopyrightText: 2015-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef SIEVEEDITORCONFIGURESERVERPAGETEST_H
#define SIEVEEDITORCONFIGURESERVERPAGETEST_H

#include <QObject>

class SieveEditorConfigureServerPageTest : public QObject
{
    Q_OBJECT
public:
    explicit SieveEditorConfigureServerPageTest(QObject *parent = nullptr);
    ~SieveEditorConfigureServerPageTest();

private Q_SLOTS:
    void shouldEmitSignalWhenClickOnConfigureButton();
    void shouldEmitSignalWhenClickOnImportSieveButton();
    void shouldHaveDefaultValue();
};

#endif // SIEVEEDITORCONFIGURESERVERPAGETEST_H
