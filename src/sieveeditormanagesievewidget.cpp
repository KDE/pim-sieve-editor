/*
   Copyright (C) 2014-2020 Laurent Montel <montel@kde.org>

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

#include "readserversieveconfigjob.h"
#include "sieveeditormanagesievewidget.h"
#include "sieveeditorutil.h"
#include <KSieveUi/SieveTreeWidgetItem>
#include <KSieveUi/ManageSieveTreeView>

#include <KManageSieve/SieveJob>

#include <KLocalizedString>

using namespace KSieveUi;

SieveEditorManageSieveWidget::SieveEditorManageSieveWidget(QWidget *parent)
    : KSieveUi::ManageSieveWidget(parent)
{
}

SieveEditorManageSieveWidget::~SieveEditorManageSieveWidget()
{
}

bool SieveEditorManageSieveWidget::refreshList()
{
    bool noImapFound = true;
    SieveTreeWidgetItem *last = nullptr;
    for (const SieveEditorUtil::SieveServerConfig &conf : mSieveServerSettings) {
        last = new SieveTreeWidgetItem(treeView(), last);
        last->setText(0, conf.sieveSettings.serverName + QStringLiteral(" (%1)").arg(conf.sieveSettings.userName));
        last->setIcon(0, QIcon::fromTheme(QStringLiteral("network-server")));

        const QUrl u = conf.url();
        if (u.isEmpty()) {
            auto *item = new QTreeWidgetItem(last);
            item->setText(0, i18n("No Sieve URL configured"));
            item->setFlags(item->flags() & ~Qt::ItemIsEnabled);
            treeView()->expandItem(last);
        } else if (!conf.enabled) {
            last->setFlags(last->flags() & ~Qt::ItemIsEnabled);
            last->setText(0, i18n("(Disabled) %1", last->text(0)));
        } else {
            KManageSieve::SieveJob *job = KManageSieve::SieveJob::list(u);
            job->setProperty("sieveimapaccountsettings", QVariant::fromValue(conf.sieveImapAccountSettings));
            connect(job, &KManageSieve::SieveJob::gotList, this, &SieveEditorManageSieveWidget::slotGotList);
            mJobs.insert(job, last);
            mUrls.insert(last, u);
            last->startAnimation();
        }
        noImapFound = false;
    }
    return noImapFound;
}

void SieveEditorManageSieveWidget::updateSieveSettings()
{
    ReadServerSieveConfigJob *job = new ReadServerSieveConfigJob(this);
    connect(job, &ReadServerSieveConfigJob::finished, this, &SieveEditorManageSieveWidget::slotReadServerSieveConfigDone);
    job->start();
}

void SieveEditorManageSieveWidget::slotReadServerSieveConfigDone(const QVector<SieveEditorUtil::SieveServerConfig> &lstConfig)
{
    mSieveServerSettings = lstConfig;
    Q_EMIT updateSieveSettingsDone();
}
