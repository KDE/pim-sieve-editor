/*
   SPDX-FileCopyrightText: 2016-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class ImportImapSettingFinishPageTest : public QObject
{
    Q_OBJECT
public:
    explicit ImportImapSettingFinishPageTest(QObject *parent = nullptr);
    ~ImportImapSettingFinishPageTest();
private Q_SLOTS:
    void shouldHaveDefaultValue();
};

