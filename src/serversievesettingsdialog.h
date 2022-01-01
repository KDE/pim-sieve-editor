/*
   SPDX-FileCopyrightText: 2013-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "sieveeditorutil.h"
#include <QDialog>
class ServerSieveSettings;
class QPushButton;
class ServerSieveSettingsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ServerSieveSettingsDialog(QWidget *parent = nullptr);
    ~ServerSieveSettingsDialog() override;

    Q_REQUIRED_RESULT QString serverName() const;
    void setServerName(const QString &name);

    Q_REQUIRED_RESULT int port() const;
    void setPort(int value);

    Q_REQUIRED_RESULT QString userName() const;
    void setUserName(const QString &name);

    Q_REQUIRED_RESULT QString password() const;
    void setPassword(const QString &pass);

    void setServerSieveConfig(const SieveEditorUtil::SieveServerConfig &conf);
    Q_REQUIRED_RESULT SieveEditorUtil::SieveServerConfig serverSieveConfig() const;

private:
    void slotEnableButtonOk(bool);
    void readConfig();
    void writeConfig();
    ServerSieveSettings *mServerSieveSettings = nullptr;
    QPushButton *mOkButton = nullptr;
};

