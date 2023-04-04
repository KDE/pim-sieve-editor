/*
   SPDX-FileCopyrightText: 2014-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "sieveeditormanagesievewidget.h"
#include "readserversieveconfigjob.h"
#include <KSieveUi/ManageSieveTreeView>
#include <KSieveUi/SieveTreeWidgetItem>

#include <KManageSieve/SieveJob>

#include <KLocalizedString>

using namespace KSieveUi;

SieveEditorManageSieveWidget::SieveEditorManageSieveWidget(QWidget *parent)
    : KSieveUi::ManageSieveWidget(parent)
{
}

SieveEditorManageSieveWidget::~SieveEditorManageSieveWidget() = default;

bool SieveEditorManageSieveWidget::refreshList()
{
    bool noImapFound = true;
    SieveTreeWidgetItem *last = nullptr;
    for (const SieveEditorUtil::SieveServerConfig &conf : std::as_const(mSieveServerSettings)) {
        last = new SieveTreeWidgetItem(treeView(), last);
        last->setText(0, conf.sieveSettings.serverName + QStringLiteral(" (%1)").arg(conf.sieveSettings.userName));
        last->setIcon(0, QIcon::fromTheme(QStringLiteral("network-server")));

        const QUrl u = conf.url();
        if (u.isEmpty()) {
            auto item = new QTreeWidgetItem(last);
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
    auto job = new ReadServerSieveConfigJob(this);
    connect(job, &ReadServerSieveConfigJob::finished, this, &SieveEditorManageSieveWidget::slotReadServerSieveConfigDone);
    job->start();
}

void SieveEditorManageSieveWidget::slotReadServerSieveConfigDone(const QList<SieveEditorUtil::SieveServerConfig> &lstConfig)
{
    mSieveServerSettings = lstConfig;
    Q_EMIT updateSieveSettingsDone();
}
