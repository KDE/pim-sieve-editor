/*
   SPDX-FileCopyrightText: 2013-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "sieveeditorutil.h"
#include <QListWidget>

class ServerSieveListWidgetItem : public QListWidgetItem
{
public:
    explicit ServerSieveListWidgetItem(QListWidget *parent = nullptr);
    ~ServerSieveListWidgetItem() override;

    [[nodiscard]] SieveEditorUtil::SieveServerConfig serverConfig() const;
    void setServerConfig(const SieveEditorUtil::SieveServerConfig &conf);

private:
    SieveEditorUtil::SieveServerConfig mServerConfig;
};

class ServerSieveListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit ServerSieveListWidget(QWidget *parent = nullptr);
    ~ServerSieveListWidget() override;

    void readConfig();
    void writeConfig();
    void addServerConfig();
    void deleteServerConfig(QListWidgetItem *item);

public Q_SLOTS:
    void modifyServerConfig();

private:
    void slotReadServerSieveConfigDone(const QList<SieveEditorUtil::SieveServerConfig> &lstConfig);
    void deletePasswords();
    QStringList mNeedToRemovePasswordInWallet;
};
