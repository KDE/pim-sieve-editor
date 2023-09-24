/*
   SPDX-FileCopyrightText: 2013-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "serversievesettings.h"
#include "sieveeditor_debug.h"
#include "ui_serversievesettings.h"
#include <KAuthorized>
#include <KLocalizedString>
#include <KMessageBox>
#include <MailTransport/ServerTest>
#include <MailTransport/Transport>

/** static helper functions **/
static QString authenticationModeString(MailTransport::Transport::EnumAuthenticationType mode)
{
    switch (mode) {
    case MailTransport::Transport::EnumAuthenticationType::LOGIN:
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
    return {};
}

static void addAuthenticationItem(QComboBox *authCombo, MailTransport::Transport::EnumAuthenticationType authtype)
{
    // qCDebug(SIEVEEDITOR_LOG) << "adding auth item " << authenticationModeString( authtype );
    authCombo->addItem(authenticationModeString(authtype), QVariant(authtype));
}

static MailTransport::Transport::EnumAuthenticationType getCurrentAuthMode(QComboBox *authCombo)
{
    auto authtype = static_cast<MailTransport::Transport::EnumAuthenticationType>(authCombo->itemData(authCombo->currentIndex()).toInt());
    // qCDebug(SIEVEEDITOR_LOG) << "current auth mode: " << authenticationModeString( authtype );
    return authtype;
}

static void setCurrentAuthMode(QComboBox *authCombo, MailTransport::Transport::EnumAuthenticationType authtype)
{
    // qCDebug(SIEVEEDITOR_LOG) << "setting authcombo: " << authenticationModeString( authtype );
    int index = authCombo->findData(authtype);
    if (index == -1) {
        qCWarning(SIEVEEDITOR_LOG) << "desired authmode not in the combo";
    }
    // qCDebug(SIEVEEDITOR_LOG) << "found corresponding index: " << index << "with data" << authenticationModeString(
    // (MailTransport::Transport::EnumAuthenticationType) authCombo->itemData( index ).toInt() );
    authCombo->setCurrentIndex(index);
    MailTransport::Transport::EnumAuthenticationType t =
        static_cast<MailTransport::Transport::EnumAuthenticationType>(authCombo->itemData(authCombo->currentIndex()).toInt());
    // qCDebug(SIEVEEDITOR_LOG) << "selected auth mode:" << authenticationModeString( t );
    Q_ASSERT(t == authtype);
}

ServerSieveSettings::ServerSieveSettings(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ServerSieveSettings)
{
    ui->setupUi(this);
    ui->serversievelabel->setMinimumSize(ui->serversievelabel->sizeHint());
    ui->testInfo->clear();
    ui->testInfo->hide();
    ui->testProgress->hide();
    ui->password->setRevealPasswordAvailable(KAuthorized::authorize(QStringLiteral("lineedit_reveal_password")));
    ui->imapPassword->setRevealPasswordAvailable(KAuthorized::authorize(QStringLiteral("lineedit_reveal_password")));

    ui->safeImapGroup->setId(ui->noRadio, KSieveCore::SieveImapAccountSettings::Unencrypted);
    ui->safeImapGroup->setId(ui->sslRadio, KSieveCore::SieveImapAccountSettings::SSLorTLS);
    ui->safeImapGroup->setId(ui->tlsRadio, KSieveCore::SieveImapAccountSettings::STARTTLS);

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
    return ui->password->password();
}

void ServerSieveSettings::setPassword(const QString &pass)
{
    ui->password->setPassword(pass);
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
    return ui->imapPassword->password();
}

void ServerSieveSettings::setImapPassword(const QString &pass)
{
    ui->imapPassword->setPassword(pass);
}

void ServerSieveSettings::setAuthenticationType(KSieveCore::SieveImapAccountSettings::AuthenticationMode type)
{
    setCurrentAuthMode(ui->imapAuthenticationCombo, static_cast<MailTransport::Transport::EnumAuthenticationType>(type));
}

KSieveCore::SieveImapAccountSettings::AuthenticationMode ServerSieveSettings::authenticationType() const
{
    return static_cast<KSieveCore::SieveImapAccountSettings::AuthenticationMode>(getCurrentAuthMode(ui->imapAuthenticationCombo));
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

    ui->safeImap->setChecked(conf.sieveImapAccountSettings.isValid());
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
    const MailTransport::Transport::EnumAuthenticationType authtype = getCurrentAuthMode(ui->authenticationCombo);
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
    if (ui->safeImap->isChecked()) {
        conf.sieveImapAccountSettings.setPort(imapPort());
        conf.sieveImapAccountSettings.setAuthenticationType(authenticationType());
        conf.sieveImapAccountSettings.setEncryptionMode(static_cast<KSieveCore::SieveImapAccountSettings::EncryptionMode>(ui->safeImapGroup->checkedId()));
    }
    return conf;
}

void ServerSieveSettings::slotTest()
{
    const QString server = imapServerName().isEmpty() ? serverName() : imapServerName();
    if (server.isEmpty()) {
        KMessageBox::error(this, i18n("Server is not defined"), i18nc("@title:window", "Check Server"));
        return;
    }
    // qCDebug(SIEVEEDITOR_LOG) << ui->imapServer->text();
    ui->testButton->setEnabled(false);
    ui->safeImap->setEnabled(false);
    ui->imapAuthenticationCombo->setEnabled(false);

    ui->testInfo->clear();
    ui->testInfo->hide();

    delete mServerTest;
    mServerTest = new MailTransport::ServerTest(this);
#ifndef QT_NO_CURSOR
    qApp->setOverrideCursor(Qt::BusyCursor);
#endif

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
        KMessageBox::error(this, i18n("Unable to connect to the server, please verify the server address."));
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
        text = i18n(
            "<qt><b>No security is supported. It is not "
            "recommended to connect to this server.</b></qt>");
    } else {
        text = i18n("<qt><b>It is not possible to use this server.</b></qt>");
    }
    ui->testInfo->setText(text);

    ui->testButton->setEnabled(true);
    ui->safeImap->setEnabled(true);
    ui->imapAuthenticationCombo->setEnabled(true);
    slotEncryptionRadioChanged();
    slotSafetyChanged();
}

void ServerSieveSettings::slotEncryptionRadioChanged()
{
    switch (ui->safeImapGroup->checkedId()) {
    case KSieveCore::SieveImapAccountSettings::Unencrypted:
    case KSieveCore::SieveImapAccountSettings::STARTTLS:
        ui->imapPort->setValue(143);
        break;
    case KSieveCore::SieveImapAccountSettings::SSLorTLS:
        ui->imapPort->setValue(993);
        break;
    default:
        qFatal("Shouldn't happen");
    }
}

void ServerSieveSettings::slotSafetyChanged()
{
    if (mServerTest == nullptr) {
        qCDebug(SIEVEEDITOR_LOG) << "serverTest null";
        ui->noRadio->setEnabled(true);
        ui->sslRadio->setEnabled(true);
        ui->tlsRadio->setEnabled(true);

        ui->imapAuthenticationCombo->setEnabled(true);
        return;
    }

    QList<int> protocols;

    switch (ui->safeImapGroup->checkedId()) {
    case KSieveCore::SieveImapAccountSettings::Unencrypted:
        qCDebug(SIEVEEDITOR_LOG) << "safeImapGroup: unencrypted";
        protocols = mServerTest->normalProtocols();
        break;
    case KSieveCore::SieveImapAccountSettings::SSLorTLS:
        protocols = mServerTest->secureProtocols();
        qCDebug(SIEVEEDITOR_LOG) << "safeImapGroup: SSL";
        break;
    case KSieveCore::SieveImapAccountSettings::STARTTLS:
        protocols = mServerTest->tlsProtocols();
        qCDebug(SIEVEEDITOR_LOG) << "safeImapGroup: starttls";
        break;
    default:
        qFatal("Shouldn't happen");
    }

    ui->imapAuthenticationCombo->clear();
    addAuthenticationItem(ui->imapAuthenticationCombo, MailTransport::Transport::EnumAuthenticationType::CLEAR);
    for (int prot : std::as_const(protocols)) {
        addAuthenticationItem(ui->imapAuthenticationCombo, static_cast<MailTransport::Transport::EnumAuthenticationType>(prot));
    }
    if (protocols.isEmpty()) {
        qCDebug(SIEVEEDITOR_LOG) << "no authmodes found";
    } else {
        setCurrentAuthMode(ui->imapAuthenticationCombo, static_cast<MailTransport::Transport::EnumAuthenticationType>(protocols.constFirst()));
    }
    mServerTest->deleteLater();
    mServerTest = nullptr;
}

#include "moc_serversievesettings.cpp"
