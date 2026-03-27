/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once
#include <TextAutoGenerateText/TextAutoGenerateTextToolInternalJob>

class AddNewScriptToolInternalJob : public TextAutoGenerateText::TextAutoGenerateTextToolInternalJob
{
    Q_OBJECT
public:
    explicit AddNewScriptToolInternalJob(QObject *parent = nullptr);
    ~AddNewScriptToolInternalJob() override;
};
