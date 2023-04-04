/*
   SPDX-FileCopyrightText: 2013-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "serversievelistwidget.h"
#include "readserversieveconfigjob.h"
#include "serversievesettingsdialog.h"
#include "sieveserversettings.h"
#include <KLocalizedString>

#include <QListWidgetItem>
#include <QPointer>

#include <qt6keychain/keychain.h>
using namespace QKeychain;
ServerSieveListWidget::ServerSieveListWidget(QWidget *parent)
    : QListWidget(parent)
{
    connect(this, &ServerSieveListWidget::itemDoubleClicked, this, &ServerSieveListWidget::modifyServerConfig);
}

ServerSieveListWidget::~ServerSieveListWidget() = default;

void ServerSieveListWidget::readConfig()
{
    auto job = new ReadServerSieveConfigJob(this);
    connect(job, &ReadServerSieveConfigJob::finished, this, &ServerSieveListWidget::slotReadServerSieveConfigDone);
    job->start();
}

void ServerSieveListWidget::slotReadServerSieveConfigDone(const QList<SieveEditorUtil::SieveServerConfig> &lstConfig)
{
    for (const SieveEditorUtil::SieveServerConfig &conf : lstConfig) {
        auto item = new ServerSieveListWidgetItem(this);
        item->setServerConfig(conf);
    }
}

void ServerSieveListWidget::writeConfig()
{
    QList<SieveEditorUtil::SieveServerConfig> lstServerConfig;
    for (int i = 0; i < count(); ++i) {
        auto serverSieveItem = static_cast<ServerSieveListWidgetItem *>(item(i));
        if (serverSieveItem) {
            SieveEditorUtil::SieveServerConfig config = serverSieveItem->serverConfig();
            config.enabled = (serverSieveItem->checkState() == Qt::Checked);
            lstServerConfig.append(config);
        }
    }
    SieveEditorUtil::writeServerSieveConfig(lstServerConfig);
    deletePasswords();
}

void ServerSieveListWidget::deletePasswords()
{
    if (!mNeedToRemovePasswordInWallet.isEmpty()) {
        for (const QString &identifier : std::as_const(mNeedToRemovePasswordInWallet)) {
            auto deleteJob = new DeletePasswordJob(QStringLiteral("sieveeditor"));
            deleteJob->setKey(identifier);
            deleteJob->start();
        }
    }
}

void ServerSieveListWidget::modifyServerConfig()
{
    QListWidgetItem *item = currentItem();
    if (!item) {
        return;
    }

    auto serverSieveListItem = static_cast<ServerSieveListWidgetItem *>(item);

    QPointer<ServerSieveSettingsDialog> dlg = new ServerSieveSettingsDialog(this);
    dlg->setWindowTitle(i18nc("@title:window", "Modify Settings"));
    dlg->setServerSieveConfig(serverSieveListItem->serverConfig());
    if (dlg->exec()) {
        serverSieveListItem->setServerConfig(dlg->serverSieveConfig());
    }
    delete dlg;
}

void ServerSieveListWidget::deleteServerConfig(QListWidgetItem *item)
{
    auto serverSieveListItem = static_cast<ServerSieveListWidgetItem *>(item);
    SieveEditorUtil::SieveServerConfig conf = serverSieveListItem->serverConfig();

    mNeedToRemovePasswordInWallet.append(SieveEditorUtil::sievePasswordIdentifier(conf.sieveSettings.userName, conf.sieveSettings.serverName));
    const QString imapIdentifier =
        SieveEditorUtil::imapPasswordIdentifier(conf.sieveImapAccountSettings.userName(), conf.sieveImapAccountSettings.serverName());
    if (!imapIdentifier.isEmpty()) {
        mNeedToRemovePasswordInWallet.append(imapIdentifier);
    }
}

void ServerSieveListWidget::addServerConfig()
{
    QPointer<ServerSieveSettingsDialog> dlg = new ServerSieveSettingsDialog(this);
    if (dlg->exec()) {
        auto item = new ServerSieveListWidgetItem(this);
        item->setServerConfig(dlg->serverSieveConfig());
    }
    delete dlg;
}

ServerSieveListWidgetItem::ServerSieveListWidgetItem(QListWidget *parent)
    : QListWidgetItem(parent)
{
}

ServerSieveListWidgetItem::~ServerSieveListWidgetItem() = default;

SieveEditorUtil::SieveServerConfig ServerSieveListWidgetItem::serverConfig() const
{
    return mServerConfig;
}

void ServerSieveListWidgetItem::setServerConfig(const SieveEditorUtil::SieveServerConfig &conf)
{
    setCheckState(conf.enabled ? Qt::Checked : Qt::Unchecked);
    setText(i18nc("server name (username)", "%1 (%2)", conf.sieveSettings.serverName, conf.sieveSettings.userName));
    mServerConfig = conf;
}
