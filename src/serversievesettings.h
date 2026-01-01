/*
   SPDX-FileCopyrightText: 2013-2026 Laurent Montel <montel@kde.org>

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

    [[nodiscard]] QString imapServerName() const;
    void setImapServerName(const QString &name);

    [[nodiscard]] int imapPort() const;
    void setImapPort(int value);

    [[nodiscard]] QString imapUserName() const;
    void setImapUserName(const QString &name);

    [[nodiscard]] QString imapPassword() const;
    void setImapPassword(const QString &pass);

    void setAuthenticationType(KSieveCore::SieveImapAccountSettings::AuthenticationMode type);
    [[nodiscard]] KSieveCore::SieveImapAccountSettings::AuthenticationMode authenticationType() const;
Q_SIGNALS:
    void enableOkButton(bool enabled);

private:
    LIBSIEVEEDITOR_NO_EXPORT void slotUserServerNameChanged();
    LIBSIEVEEDITOR_NO_EXPORT void slotTest();

    LIBSIEVEEDITOR_NO_EXPORT void slotFinished(const QList<int> &testResult);
    LIBSIEVEEDITOR_NO_EXPORT void slotEncryptionRadioChanged();

    LIBSIEVEEDITOR_NO_EXPORT void populateDefaultAuthenticationOptions(QComboBox *combobox);
    LIBSIEVEEDITOR_NO_EXPORT void slotSafetyChanged();
    LIBSIEVEEDITOR_NO_EXPORT void populateDefaultComboBoxAuthenticationOptions();
    Ui::ServerSieveSettings *const ui;
    MailTransport::ServerTest *mServerTest = nullptr;
};
