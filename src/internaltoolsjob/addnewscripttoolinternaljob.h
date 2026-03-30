/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once
#include <TextAutoGenerateText/TextAutoGenerateTextToolInternalJob>
class SieveEditorMainWidget;
class AddNewScriptToolInternalJob : public TextAutoGenerateText::TextAutoGenerateTextToolInternalJob
{
    Q_OBJECT
public:
    explicit AddNewScriptToolInternalJob(SieveEditorMainWidget *sieveEditorMainWidget, QObject *parent = nullptr);
    ~AddNewScriptToolInternalJob() override;
    [[nodiscard]] QByteArray toolName() const override;
    [[nodiscard]] static QByteArray toolId();

    void start() override;

private:
    SieveEditorMainWidget *const mSieveEditorMainWidget;
};
