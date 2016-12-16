/*
   Copyright (C) 2013-2016 Montel Laurent <montel@kde.org>

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

#include "serversievesettings.h"
#include "ui_serversievesettings.h"
#include <MailTransport/mailtransport/transport.h>
#include <MailTransport/ServerTest>
#include <KLocalizedString>
#include <KMessageBox>
#include "sieveeditor_debug.h"

/** static helper functions **/
static QString authenticationModeString(MailTransport::Transport::EnumAuthenticationType::type mode)
{
    switch (mode) {
    case  MailTransport::Transport::EnumAuthenticationType::LOGIN:
        return QStringLiteral("LOGIN");
    case MailTransport::Transport::EnumAuthenticationType::PLAIN:
        return QStringLiteral("PLAIN");
    case MailTransport::Transport::EnumAuthenticationType::CRAM_MD5:
        return QStringLiteral("CRAM-MD5");
    case MailTransport::Transport::EnumAuthenticationType::DIGEST_MD5:
        return QStringLiteral("DIGEST-MD5");
    case MailTransport::Transport::EnumAuthenticationType::GSSAPI:
        return QStringLiteral("GSSAPI");
    case MailTransport::Transport::EnumAuthenticationType::NTLM:
        return QStringLiteral("NTLM");
    case MailTransport::Transport::EnumAuthenticationType::CLEAR:
        return i18nc("Authentication method", "Clear text");
    case MailTransport::Transport::EnumAuthenticationType::ANONYMOUS:
        return i18nc("Authentication method", "Anonymous");
    default:
        break;
    }
    return QString();
}

static void addAuthenticationItem(QComboBox *authCombo, MailTransport::Transport::EnumAuthenticationType::type authtype)
{
    //qCDebug(SIEVEEDITOR_LOG) << "adding auth item " << authenticationModeString( authtype );
    authCombo->addItem(authenticationModeString(authtype), QVariant(authtype));
}

static MailTransport::Transport::EnumAuthenticationType::type getCurrentAuthMode(QComboBox *authCombo)
{
    MailTransport::Transport::EnumAuthenticationType::type authtype = (MailTransport::Transport::EnumAuthenticationType::type) authCombo->itemData(authCombo->currentIndex()).toInt();
    //qCDebug(SIEVEEDITOR_LOG) << "current auth mode: " << authenticationModeString( authtype );
    return authtype;
}

static void setCurrentAuthMode(QComboBox *authCombo, MailTransport::Transport::EnumAuthenticationType::type authtype)
{
    //qCDebug(SIEVEEDITOR_LOG) << "setting authcombo: " << authenticationModeString( authtype );
    int index = authCombo->findData(authtype);
    if (index == -1) {
        qCWarning(SIEVEEDITOR_LOG) << "desired authmode not in the combo";
    }
    //qCDebug(SIEVEEDITOR_LOG) << "found corresponding index: " << index << "with data" << authenticationModeString( (MailTransport::Transport::EnumAuthenticationType::type) authCombo->itemData( index ).toInt() );
    authCombo->setCurrentIndex(index);
    MailTransport::Transport::EnumAuthenticationType::type t = (MailTransport::Transport::EnumAuthenticationType::type) authCombo->itemData(authCombo->currentIndex()).toInt();
    //qCDebug(SIEVEEDITOR_LOG) << "selected auth mode:" << authenticationModeString( t );
    Q_ASSERT(t == authtype);
}

ServerSieveSettings::ServerSieveSettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ServerSieveSettings),
    mServerTest(Q_NULLPTR)
{
    ui->setupUi(this);
    ui->serversievelabel->setMinimumSize(ui->serversievelabel->sizeHint());
    ui->testInfo->clear();
    ui->testInfo->hide();
    ui->testProgress->hide();

    ui->safeImapGroup->setId(ui->noRadio, KSieveUi::SieveImapAccountSettings::Unencrypted);
    ui->safeImapGroup->setId(ui->sslRadio, KSieveUi::SieveImapAccountSettings::AnySslVersion);
    ui->safeImapGroup->setId(ui->tlsRadio, KSieveUi::SieveImapAccountSettings::TlsV1);


    connect(ui->testButton, &QPushButton::pressed, this, &ServerSieveSettings::slotTest);

    populateDefaultComboBoxAuthenticationOptions();
    connect(ui->serverName, &QLineEdit::textChanged, this, &ServerSieveSettings::slotUserServerNameChanged);
    connect(ui->userName, &QLineEdit::textChanged, this, &ServerSieveSettings::slotUserServerNameChanged);
}

ServerSieveSettings::~ServerSieveSettings()
{
    delete ui;
}


void ServerSieveSettings::populateDefaultComboBoxAuthenticationOptions()
{
    populateDefaultAuthenticationOptions(ui->authenticationCombo);
    populateDefaultAuthenticationOptions(ui->imapAuthenticationCombo);
}

void ServerSieveSettings::populateDefaultAuthenticationOptions(QComboBox *combobox)
{
    combobox->clear();
    addAuthenticationItem(combobox, MailTransport::Transport::EnumAuthenticationType::CLEAR);
    addAuthenticationItem(combobox, MailTransport::Transport::EnumAuthenticationType::LOGIN);
    addAuthenticationItem(combobox, MailTransport::Transport::EnumAuthenticationType::PLAIN);
    addAuthenticationItem(combobox, MailTransport::Transport::EnumAuthenticationType::CRAM_MD5);
    addAuthenticationItem(combobox, MailTransport::Transport::EnumAuthenticationType::DIGEST_MD5);
    addAuthenticationItem(combobox, MailTransport::Transport::EnumAuthenticationType::NTLM);
    addAuthenticationItem(combobox, MailTransport::Transport::EnumAuthenticationType::GSSAPI);
    addAuthenticationItem(combobox, MailTransport::Transport::EnumAuthenticationType::ANONYMOUS);
}

void ServerSieveSettings::slotUserServerNameChanged()
{
    Q_EMIT enableOkButton(!ui->userName->text().trimmed().isEmpty() && !ui->serverName->text().trimmed().isEmpty());
}

QString ServerSieveSettings::serverName() const
{
    return ui->serverName->text().trimmed();
}

void ServerSieveSettings::setServerName(const QString &name)
{
    ui->serverName->setText(name);
}

int ServerSieveSettings::port() const
{
    return ui->port->value();
}

void ServerSieveSettings::setPort(int value)
{
    ui->port->setValue(value);
}

QString ServerSieveSettings::userName() const
{
    return ui->userName->text().trimmed();
}

void ServerSieveSettings::setUserName(const QString &name)
{
    ui->userName->setText(name);
}

QString ServerSieveSettings::password() const
{
    return ui->password->text();
}

void ServerSieveSettings::setPassword(const QString &pass)
{
    ui->password->setText(pass);
}

QString ServerSieveSettings::imapServerName() const
{
    return ui->imapServerName->text().trimmed();
}

void ServerSieveSettings::setImapServerName(const QString &name)
{
    ui->imapServerName->setText(name);
}

int ServerSieveSettings::imapPort() const
{
    return ui->imapPort->value();
}

void ServerSieveSettings::setImapPort(int value)
{
    ui->imapPort->setValue(value);
}

QString ServerSieveSettings::imapUserName() const
{
    return ui->imapUserName->text().trimmed();
}

void ServerSieveSettings::setImapUserName(const QString &name)
{
    ui->imapUserName->setText(name);
}

QString ServerSieveSettings::imapPassword() const
{
    return ui->imapPassword->text();
}

void ServerSieveSettings::setImapPassword(const QString &pass)
{
    ui->imapPassword->setText(pass);
}

void ServerSieveSettings::setAuthenticationType(KSieveUi::SieveImapAccountSettings::AuthenticationMode type)
{
    setCurrentAuthMode(ui->imapAuthenticationCombo, static_cast<MailTransport::Transport::EnumAuthenticationType::type>(type));
}

KSieveUi::SieveImapAccountSettings::AuthenticationMode ServerSieveSettings::authenticationType() const
{
    return static_cast<KSieveUi::SieveImapAccountSettings::AuthenticationMode>(getCurrentAuthMode(ui->imapAuthenticationCombo));
}

void ServerSieveSettings::setServerSieveConfig(const SieveEditorUtil::SieveServerConfig &conf)
{
    setPassword(conf.sieveSettings.password);
    setPort(conf.sieveSettings.port);
    setServerName(conf.sieveSettings.serverName);
    setUserName(conf.sieveSettings.userName);
    setCurrentAuthMode(ui->authenticationCombo, conf.sieveSettings.authenticationType);
    ui->alternateServer->setChecked(conf.useImapCustomServer);
    if (conf.useImapCustomServer) {
        setImapPassword(conf.sieveImapAccountSettings.password());
        setImapUserName(conf.sieveImapAccountSettings.userName());
        setImapServerName(conf.sieveImapAccountSettings.serverName());
    }
    setImapPort(conf.sieveImapAccountSettings.port());
    setAuthenticationType(conf.sieveImapAccountSettings.authenticationType());

    QAbstractButton *safetyButton = ui->safeImapGroup->button(conf.sieveImapAccountSettings.encryptionMode());
    if (safetyButton) {
        safetyButton->setChecked(true);
    }
}

SieveEditorUtil::SieveServerConfig ServerSieveSettings::serverSieveConfig() const
{
    SieveEditorUtil::SieveServerConfig conf;
    conf.sieveSettings.password = password();
    conf.sieveSettings.port = port();
    conf.sieveSettings.serverName = serverName();
    conf.sieveSettings.userName = userName();
    const MailTransport::Transport::EnumAuthenticationType::type authtype = getCurrentAuthMode(ui->authenticationCombo);
    conf.sieveSettings.authenticationType = authtype;

    conf.useImapCustomServer = ui->alternateServer->isChecked();
    if (ui->alternateServer->isChecked()) {
        conf.sieveImapAccountSettings.setPassword(imapPassword());
        conf.sieveImapAccountSettings.setUserName(imapUserName());
        conf.sieveImapAccountSettings.setServerName(imapServerName());
    } else {
        conf.sieveImapAccountSettings.setPassword(password());
        conf.sieveImapAccountSettings.setUserName(userName());
        conf.sieveImapAccountSettings.setServerName(serverName());
    }
    conf.sieveImapAccountSettings.setPort(imapPort());
    conf.sieveImapAccountSettings.setAuthenticationType(authenticationType());
    conf.sieveImapAccountSettings.setEncryptionMode(static_cast<KSieveUi::SieveImapAccountSettings::EncryptionMode>(ui->safeImapGroup->checkedId()));
    return conf;
}

void ServerSieveSettings::slotTest()
{
    //qCDebug(SIEVEEDITOR_LOG) << ui->imapServer->text();
    ui->testButton->setEnabled(false);
    ui->safeImap->setEnabled(false);
    ui->authenticationCombo->setEnabled(false);

    ui->testInfo->clear();
    ui->testInfo->hide();

    delete mServerTest;
    mServerTest = new MailTransport::ServerTest(this);
#ifndef QT_NO_CURSOR
    qApp->setOverrideCursor(Qt::BusyCursor);
#endif

    const QString server = imapServerName().isEmpty() ? serverName() : imapServerName();
    const int portValue = ui->imapPort->value();
    qCDebug(SIEVEEDITOR_LOG) << "server: " << server << "port: " << portValue;

    mServerTest->setServer(server);

    if (portValue != 143 && portValue != 993) {
        mServerTest->setPort(MailTransport::Transport::EnumEncryption::None, portValue);
        mServerTest->setPort(MailTransport::Transport::EnumEncryption::SSL, portValue);
    }

    mServerTest->setProtocol(QStringLiteral("imap"));
    mServerTest->setProgressBar(ui->testProgress);
    connect(mServerTest, &MailTransport::ServerTest::finished, this, &ServerSieveSettings::slotFinished);
    mServerTest->start();
}

void ServerSieveSettings::slotFinished(const QList<int> &testResult)
{
    qCDebug(SIEVEEDITOR_LOG) << testResult;

#ifndef QT_NO_CURSOR
    qApp->restoreOverrideCursor();
#endif
    using namespace MailTransport;

    if (!mServerTest->isNormalPossible() && !mServerTest->isSecurePossible()) {
        KMessageBox::sorry(this, i18n("Unable to connect to the server, please verify the server address."));
    }

    ui->testInfo->show();

    ui->sslRadio->setEnabled(testResult.contains(Transport::EnumEncryption::SSL));
    ui->tlsRadio->setEnabled(testResult.contains(Transport::EnumEncryption::TLS));
    ui->noRadio->setEnabled(testResult.contains(Transport::EnumEncryption::None));

    QString text;
    if (testResult.contains(Transport::EnumEncryption::TLS)) {
        ui->tlsRadio->setChecked(true);
        text = i18n("<qt><b>TLS is supported and recommended.</b></qt>");
    } else if (testResult.contains(Transport::EnumEncryption::SSL)) {
        ui->sslRadio->setChecked(true);
        text = i18n("<qt><b>SSL is supported and recommended.</b></qt>");
    } else if (testResult.contains(Transport::EnumEncryption::None)) {
        ui->noRadio->setChecked(true);
        text = i18n("<qt><b>No security is supported. It is not "
                    "recommended to connect to this server.</b></qt>");
    } else {
        text = i18n("<qt><b>It is not possible to use this server.</b></qt>");
    }
    ui->testInfo->setText(text);

    ui->testButton->setEnabled(true);
    ui->safeImap->setEnabled(true);
    ui->authenticationCombo->setEnabled(true);
    slotEncryptionRadioChanged();
    slotSafetyChanged();
}

void ServerSieveSettings::slotEncryptionRadioChanged()
{
    switch (ui->safeImapGroup->checkedId()) {
    case KSieveUi::SieveImapAccountSettings::Unencrypted:
    case KSieveUi::SieveImapAccountSettings::TlsV1:
        ui->imapPort->setValue(143);
        break;
    case KSieveUi::SieveImapAccountSettings::AnySslVersion:
        ui->imapPort->setValue(993);
        break;
    default:
        qFatal("Shouldn't happen");
    }
}

void ServerSieveSettings::slotSafetyChanged()
{
    if (mServerTest == Q_NULLPTR) {
        qCDebug(SIEVEEDITOR_LOG) << "serverTest null";
        ui->noRadio->setEnabled(true);
        ui->sslRadio->setEnabled(true);
        ui->tlsRadio->setEnabled(true);

        ui->authenticationCombo->setEnabled(true);
        return;
    }

    QList<int> protocols;

    switch (ui->safeImapGroup->checkedId()) {
    case KSieveUi::SieveImapAccountSettings::Unencrypted :
        qCDebug(SIEVEEDITOR_LOG) << "safeImapGroup: unencrypted";
        protocols = mServerTest->normalProtocols();
        break;
    case KSieveUi::SieveImapAccountSettings::AnySslVersion:
        protocols = mServerTest->secureProtocols();
        qCDebug(SIEVEEDITOR_LOG) << "safeImapGroup: SSL";
        break;
    case KSieveUi::SieveImapAccountSettings::TlsV1:
        protocols = mServerTest->tlsProtocols();
        qCDebug(SIEVEEDITOR_LOG) << "safeImapGroup: starttls";
        break;
    default:
        qFatal("Shouldn't happen");
    }

    ui->authenticationCombo->clear();
    addAuthenticationItem(ui->imapAuthenticationCombo, MailTransport::Transport::EnumAuthenticationType::CLEAR);
    foreach (int prot, protocols) {
        addAuthenticationItem(ui->imapAuthenticationCombo, (MailTransport::Transport::EnumAuthenticationType::type) prot);
    }
    if (protocols.isEmpty()) {
        qCDebug(SIEVEEDITOR_LOG) << "no authmodes found";
    } else {
        setCurrentAuthMode(ui->imapAuthenticationCombo, (MailTransport::Transport::EnumAuthenticationType::type) protocols.first());
    }
}
