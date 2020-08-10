/*
   SPDX-FileCopyrightText: 2013-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef SERVERSIEVELISTWIDGET_H
#define SERVERSIEVELISTWIDGET_H

#include <QListWidget>
#include "sieveeditorutil.h"

class ServerSieveListWidgetItem : public QListWidgetItem
{
public:
    explicit ServerSieveListWidgetItem(QListWidget *parent = nullptr);
    ~ServerSieveListWidgetItem();

    Q_REQUIRED_RESULT SieveEditorUtil::SieveServerConfig serverConfig() const;
    void setServerConfig(const SieveEditorUtil::SieveServerConfig &conf);

private:
    SieveEditorUtil::SieveServerConfig mServerConfig;
};

class ServerSieveListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit ServerSieveListWidget(QWidget *parent = nullptr);
    ~ServerSieveListWidget();

    void readConfig();
    void writeConfig();
    void addServerConfig();
    void deleteServerConfig(QListWidgetItem *item);

public Q_SLOTS:
    void modifyServerConfig();

private:
    QStringList mNeedToRemovePasswordInWallet;
};

#endif // SERVERSIEVELISTWIDGET_H
