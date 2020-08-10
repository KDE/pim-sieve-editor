/*
   SPDX-FileCopyrightText: 2013-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "serversievelistwidget.h"
#include "sieveeditorutil.h"
#include "serversievesettingsdialog.h"

#include <KLocalizedString>

#include <QListWidgetItem>
#include <QPointer>

ServerSieveListWidget::ServerSieveListWidget(QWidget *parent)
    : QListWidget(parent)
{
    connect(this, &ServerSieveListWidget::itemDoubleClicked, this, &ServerSieveListWidget::modifyServerConfig);
}

ServerSieveListWidget::~ServerSieveListWidget()
{
}

void ServerSieveListWidget::readConfig()
{
    const QVector<SieveEditorUtil::SieveServerConfig> lstServer = SieveEditorUtil::readServerSieveConfig();
    for (const SieveEditorUtil::SieveServerConfig &conf : lstServer) {
        ServerSieveListWidgetItem *item = new ServerSieveListWidgetItem(this);
        item->setServerConfig(conf);
    }
}

void ServerSieveListWidget::writeConfig()
{
    QVector<SieveEditorUtil::SieveServerConfig> lstServerConfig;
    for (int i = 0; i < count(); ++i) {
        ServerSieveListWidgetItem *serverSieveItem = static_cast<ServerSieveListWidgetItem *>(item(i));
        if (serverSieveItem) {
            SieveEditorUtil::SieveServerConfig config = serverSieveItem->serverConfig();
            config.enabled = (serverSieveItem->checkState() == Qt::Checked);
            lstServerConfig.append(config);
        }
    }
    SieveEditorUtil::writeServerSieveConfig(lstServerConfig);
    SieveEditorUtil::deletePasswords(mNeedToRemovePasswordInWallet);
}

void ServerSieveListWidget::modifyServerConfig()
{
    QListWidgetItem *item = currentItem();
    if (!item) {
        return;
    }

    ServerSieveListWidgetItem *serverSieveListItem = static_cast<ServerSieveListWidgetItem *>(item);

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
    ServerSieveListWidgetItem *serverSieveListItem = static_cast<ServerSieveListWidgetItem *>(item);
    SieveEditorUtil::SieveServerConfig conf = serverSieveListItem->serverConfig();

    mNeedToRemovePasswordInWallet.append(SieveEditorUtil::sievePasswordIdentifier(conf.sieveSettings.userName, conf.sieveSettings.serverName));
    const QString imapIdentifier = SieveEditorUtil::imapPasswordIdentifier(conf.sieveImapAccountSettings.userName(), conf.sieveImapAccountSettings.serverName());
    if (!imapIdentifier.isEmpty()) {
        mNeedToRemovePasswordInWallet.append(imapIdentifier);
    }
}

void ServerSieveListWidget::addServerConfig()
{
    QPointer<ServerSieveSettingsDialog> dlg = new ServerSieveSettingsDialog(this);
    if (dlg->exec()) {
        ServerSieveListWidgetItem *item = new ServerSieveListWidgetItem(this);
        item->setServerConfig(dlg->serverSieveConfig());
    }
    delete dlg;
}

ServerSieveListWidgetItem::ServerSieveListWidgetItem(QListWidget *parent)
    : QListWidgetItem(parent)
{
}

ServerSieveListWidgetItem::~ServerSieveListWidgetItem()
{
}

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
