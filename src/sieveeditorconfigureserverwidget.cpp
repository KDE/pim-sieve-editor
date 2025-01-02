/*
   SPDX-FileCopyrightText: 2013-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "sieveeditorconfigureserverwidget.h"
#include "serversievesettingsdialog.h"
#include "ui_sieveeditorconfigureserverwidget.h"

#include <KLocalizedString>
#include <KMessageBox>

SieveEditorConfigureServerWidget::SieveEditorConfigureServerWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SieveEditorConfigureServerWidget)
{
    ui->setupUi(this);
    connect(ui->modifyServer, &QPushButton::clicked, this, &SieveEditorConfigureServerWidget::slotModifyServer);
    connect(ui->addServer, &QPushButton::clicked, this, &SieveEditorConfigureServerWidget::slotAddServer);
    connect(ui->removeServer, &QPushButton::clicked, this, &SieveEditorConfigureServerWidget::slotDeleteServer);
    connect(ui->serverSieveListWidget, &ServerSieveListWidget::itemSelectionChanged, this, &SieveEditorConfigureServerWidget::slotItemSelectionChanged);
    slotItemSelectionChanged();
}

SieveEditorConfigureServerWidget::~SieveEditorConfigureServerWidget()
{
    delete ui;
}

void SieveEditorConfigureServerWidget::readConfig()
{
    ui->serverSieveListWidget->readConfig();
}

void SieveEditorConfigureServerWidget::writeConfig()
{
    ui->serverSieveListWidget->writeConfig();
}

void SieveEditorConfigureServerWidget::slotModifyServer()
{
    ui->serverSieveListWidget->modifyServerConfig();
}

void SieveEditorConfigureServerWidget::slotAddServer()
{
    ui->serverSieveListWidget->addServerConfig();
}

void SieveEditorConfigureServerWidget::slotDeleteServer()
{
    QListWidgetItem *item = ui->serverSieveListWidget->currentItem();
    if (!item) {
        return;
    }
    if (KMessageBox::ButtonCode::PrimaryAction
        == KMessageBox::questionTwoActions(this,
                                           i18n("Do you want to remove this server \'%1\'?", item->text()),
                                           i18nc("@title:window", "Remove Sieve Server"),
                                           KStandardGuiItem::remove(),
                                           KStandardGuiItem::cancel())) {
        ui->serverSieveListWidget->deleteServerConfig(item);
        delete item;
        slotItemSelectionChanged();
    }
}

void SieveEditorConfigureServerWidget::slotItemSelectionChanged()
{
    const bool hasItemSelected = ui->serverSieveListWidget->currentItem();
    ui->modifyServer->setEnabled(hasItemSelected);
    ui->removeServer->setEnabled(hasItemSelected);
}

#include "moc_sieveeditorconfigureserverwidget.cpp"
