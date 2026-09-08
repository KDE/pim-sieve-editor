/*
   SPDX-FileCopyrightText: 2013-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "serversievesettingsdialog.h"
#include "serversievesettings.h"

#include <KLocalizedString>
#include <KWindowConfig>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWindow>
#include <TextAddonsWidgets/LoadDialogSizeUtils>

namespace
{
static const char myServerSieveSettingsDialog[] = "ServerSieveSettingsDialog";
}

ServerSieveSettingsDialog::ServerSieveSettingsDialog(QWidget *parent)
    : QDialog(parent)
    , mServerSieveSettings(new ServerSieveSettings(this))
{
    setWindowTitle(i18nc("@title:window", "Add Sieve Server"));

    auto w = new QWidget;
    auto lay = new QVBoxLayout;
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
#if TEXTADDONSWIDGETS_VERSION >= QT_VERSION_CHECK(2, 1, 49)
    TextAddonsWidgets::LoadDialogSizeUtils::manageDialogSize(this, QLatin1StringView(myServerSieveSettingsDialog), QSize(450, 350));
#else
    create(); // ensure a window is created
    windowHandle()->resize(QSize(450, 350));
    const KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myServerSieveSettingsDialog));
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
#endif
}

void ServerSieveSettingsDialog::writeConfig()
{
#if TEXTADDONSWIDGETS_VERSION < QT_VERSION_CHECK(2, 1, 49)
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myServerSieveSettingsDialog));
    KWindowConfig::saveWindowSize(windowHandle(), group);
    group.sync();
#endif
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

#include "moc_serversievesettingsdialog.cpp"
