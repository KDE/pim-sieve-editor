/*
   SPDX-FileCopyrightText: 2016-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class ImportImapSettingSearchPageTest : public QObject
{
    Q_OBJECT
public:
    explicit ImportImapSettingSearchPageTest(QObject *parent = nullptr);
    ~ImportImapSettingSearchPageTest() override;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldAddCheckPrograms();
};

