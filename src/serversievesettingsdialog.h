/*
   SPDX-FileCopyrightText: 2013-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QDialog>
class ServerSieveSettings;
class QPushButton;
namespace SieveEditorUtil
{
struct SieveServerConfig;
}
class ServerSieveSettingsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ServerSieveSettingsDialog(QWidget *parent = nullptr);
    ~ServerSieveSettingsDialog() override;

    [[nodiscard]] QString serverName() const;
    void setServerName(const QString &name);

    [[nodiscard]] int port() const;
    void setPort(int value);

    [[nodiscard]] QString userName() const;
    void setUserName(const QString &name);

    [[nodiscard]] QString password() const;
    void setPassword(const QString &pass);

    void setServerSieveConfig(const SieveEditorUtil::SieveServerConfig &conf);
    [[nodiscard]] SieveEditorUtil::SieveServerConfig serverSieveConfig() const;

private:
    void slotEnableButtonOk(bool);
    void readConfig();
    void writeConfig();
    ServerSieveSettings *const mServerSieveSettings;
    QPushButton *mOkButton = nullptr;
};
