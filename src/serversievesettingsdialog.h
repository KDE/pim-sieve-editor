/*
   Copyright (C) 2013-2020 Laurent Montel <montel@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef SERVERSIEVESETTINGSDIALOG_H
#define SERVERSIEVESETTINGSDIALOG_H

#include <QDialog>
#include "sieveeditorutil.h"
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

#endif // SERVERSIEVESETTINGSDIALOG_H
