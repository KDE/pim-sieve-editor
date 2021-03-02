/*
   SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "sieveeditorutil.h"
#include <KSieveUi/ManageSieveWidget>

class SieveEditorManageSieveWidget : public KSieveUi::ManageSieveWidget
{
    Q_OBJECT
public:
    explicit SieveEditorManageSieveWidget(QWidget *parent = nullptr);
    ~SieveEditorManageSieveWidget() override;

protected:
    Q_REQUIRED_RESULT bool refreshList() override;
    void updateSieveSettings() override;

private:
    void slotReadServerSieveConfigDone(const QVector<SieveEditorUtil::SieveServerConfig> &lstConfig);
    QVector<SieveEditorUtil::SieveServerConfig> mSieveServerSettings;
};

