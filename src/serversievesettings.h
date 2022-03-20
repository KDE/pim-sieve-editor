/*
   SPDX-FileCopyrightText: 2013-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "libsieveeditor_private_export.h"
#include "sieveeditorutil.h"
#include <QWidget>

namespace Ui
{
class ServerSieveSettings;
}
namespace MailTransport
{
class ServerTest;
}
class QComboBox;
class LIBSIEVEEDITOR_TESTS_EXPORT ServerSieveSettings : public QWidget
{
    Q_OBJECT

public:
    explicit ServerSieveSettings(QWidget *parent = nullptr);
    ~ServerSieveSettings() override;

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
