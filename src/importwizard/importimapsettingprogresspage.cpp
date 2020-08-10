/*
   SPDX-FileCopyrightText: 2016-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "importimapsettingprogresspage.h"
#include "sieveeditor_debug.h"
#include "checks/abstractimapsettingscheckjob.h"
#include <QVBoxLayout>
#include <KLocalizedString>
#include <QTextEdit>

ImportImapSettingProgressPage::ImportImapSettingProgressPage(QWidget *parent)
    : QWidget(parent)
    , mSettingsFound(false)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainlayout"));
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mProgressTextEdit = new QTextEdit(this);
    mProgressTextEdit->setObjectName(QStringLiteral("textedit"));
    mProgressTextEdit->setReadOnly(true);
    mainLayout->addWidget(mProgressTextEdit);
}

ImportImapSettingProgressPage::~ImportImapSettingProgressPage()
{
}

void ImportImapSettingProgressPage::addProgressInfo(const QString &str)
{
    mProgressTextEdit->append(str);
}

void ImportImapSettingProgressPage::setSelectedPrograms(const QStringList &programs)
{
    mSelectedPrograms = programs;
}

void ImportImapSettingProgressPage::setListCheckJob(const QMap<QString, AbstractImapSettingsCheckJob *> &listCheckJob)
{
    mListCheckJob = listCheckJob;
}

void ImportImapSettingProgressPage::start()
{
    if (mSelectedPrograms.isEmpty()) {
        qCDebug(SIEVEEDITOR_LOG) << "No selected import type.";
        Q_EMIT finished();
        return;
    }
    if (mListCheckJob.isEmpty()) {
        qCWarning(SIEVEEDITOR_LOG) << "It's a bug, list of check job can not be empty";
        Q_EMIT finished();
        return;
    }
    for (const QString &prg : qAsConst(mSelectedPrograms)) {
        AbstractImapSettingsCheckJob *job = mListCheckJob.value(prg);
        connect(job, &AbstractImapSettingsCheckJob::importSetting, this, &ImportImapSettingProgressPage::slotImportSettingsDone);
        connect(job, &AbstractImapSettingsCheckJob::noSettingsImported, this, &ImportImapSettingProgressPage::noSettingsImported);
        job->start();
    }
    if (!mSettingsFound) {
        addProgressInfo(i18n("No settings found."));
    }
    Q_EMIT finished();
}

void ImportImapSettingProgressPage::slotImportSettingsDone(const QString &name, const SieveEditorUtil::SieveServerConfig &settings)
{
    const QString str{
        i18n("Import settings from %1...", name)
    };
    addProgressInfo(str);
    const QString summaryStr{
        i18n("Import settings from %1 done", name)
    };
    Q_EMIT addSummaryInfo(summaryStr);
    SieveEditorUtil::addServerSieveConfig(settings);
    mSettingsFound = true;
}
