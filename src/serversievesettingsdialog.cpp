/*
   SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "serversievesettingsdialog.h"
#include "serversievesettings.h"

#include <KLocalizedString>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>

ServerSieveSettingsDialog::ServerSieveSettingsDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(i18nc("@title:window", "Add Sieve Server"));

    auto w = new QWidget;
    auto lay = new QVBoxLayout;
    mServerSieveSettings = new ServerSieveSettings;
    connect(mServerSieveSettings, &ServerSieveSettings::enableOkButton, this, &ServerSieveSettingsDialog::slotEnableButtonOk);
    lay->addWidget(mServerSieveSettings);
    lay->setContentsMargins({});
    w->setLayout(lay);
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(w);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    mOkButton = buttonBox->button(QDialogButtonBox::Ok);
    mOkButton->setDefault(true);
    mOkButton->setShortcut(Qt::CTRL | Qt::Key_Return);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &ServerSieveSettingsDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &ServerSieveSettingsDialog::reject);
    mainLayout->addWidget(buttonBox);

    readConfig();
    mOkButton->setEnabled(false);
}

ServerSieveSettingsDialog::~ServerSieveSettingsDialog()
{
    writeConfig();
}

void ServerSieveSettingsDialog::readConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), "ServerSieveSettingsDialog");
    const QSize size = group.readEntry("Size", QSize(450, 350));
    if (size.isValid()) {
        resize(size);
    }
}

void ServerSieveSettingsDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), "ServerSieveSettingsDialog");
    group.writeEntry("Size", size());
    group.sync();
}

void ServerSieveSettingsDialog::slotEnableButtonOk(bool b)
{
    mOkButton->setEnabled(b);
}

QString ServerSieveSettingsDialog::serverName() const
{
    return mServerSieveSettings->serverName();
}

void ServerSieveSettingsDialog::setServerName(const QString &name)
{
    mServerSieveSettings->setServerName(name);
}

int ServerSieveSettingsDialog::port() const
{
    return mServerSieveSettings->port();
}

void ServerSieveSettingsDialog::setPort(int value)
{
    mServerSieveSettings->setPort(value);
}

QString ServerSieveSettingsDialog::userName() const
{
    return mServerSieveSettings->userName();
}

void ServerSieveSettingsDialog::setUserName(const QString &name)
{
    mServerSieveSettings->setUserName(name);
}

QString ServerSieveSettingsDialog::password() const
{
    return mServerSieveSettings->password();
}

void ServerSieveSettingsDialog::setPassword(const QString &pass)
{
    mServerSieveSettings->setPassword(pass);
}

void ServerSieveSettingsDialog::setServerSieveConfig(const SieveEditorUtil::SieveServerConfig &conf)
{
    mServerSieveSettings->setServerSieveConfig(conf);
}

SieveEditorUtil::SieveServerConfig ServerSieveSettingsDialog::serverSieveConfig() const
{
    return mServerSieveSettings->serverSieveConfig();
}
