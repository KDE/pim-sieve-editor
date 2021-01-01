/*
   Copyright (C) 2013-2021 Laurent Montel <montel@kde.org>

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

#ifndef SERVERSIEVESETTINGS_H
#define SERVERSIEVESETTINGS_H

#include <QWidget>
#include "sieveeditorutil.h"
#include "libsieveeditor_private_export.h"

namespace Ui {
class ServerSieveSettings;
}
namespace MailTransport {
class ServerTest;
}
class QComboBox;
class LIBSIEVEEDITOR_TESTS_EXPORT ServerSieveSettings : public QWidget
{
    Q_OBJECT

public:
    explicit ServerSieveSettings(QWidget *parent = nullptr);
    ~ServerSieveSettings();

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

    Q_REQUIRED_RESULT QString imapServerName() const;
    void setImapServerName(const QString &name);

    Q_REQUIRED_RESULT int imapPort() const;
    void setImapPort(int value);

    Q_REQUIRED_RESULT QString imapUserName() const;
    void setImapUserName(const QString &name);

    Q_REQUIRED_RESULT QString imapPassword() const;
    void setImapPassword(const QString &pass);

    void setAuthenticationType(KSieveUi::SieveImapAccountSettings::AuthenticationMode type);
    Q_REQUIRED_RESULT KSieveUi::SieveImapAccountSettings::AuthenticationMode authenticationType() const;
Q_SIGNALS:
    void enableOkButton(bool enabled);

private:
    void slotUserServerNameChanged();
    void slotTest();

    void slotFinished(const QVector<int> &testResult);
    void slotEncryptionRadioChanged();
private:
    void populateDefaultAuthenticationOptions(QComboBox *combobox);
    void slotSafetyChanged();
    void populateDefaultComboBoxAuthenticationOptions();
    Ui::ServerSieveSettings *const ui;
    MailTransport::ServerTest *mServerTest = nullptr;
};

#endif // SERVERSIEVESETTINGS_H
