/*
   Copyright (C) 2016-2017 Montel Laurent <montel@kde.org>

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

#include "importimapsettingprogresspage.h"
#include "sieveeditor_debug.h"
#include "checks/abstractimapsettingscheckjob.h"
#include "helper_p.h"
#include <QVBoxLayout>
#include <KLocalizedString>
#include <QTextEdit>

ImportImapSettingProgressPage::ImportImapSettingProgressPage(QWidget *parent)
    : QWidget(parent),
      mSettingsFound(false)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainlayout"));
    mainLayout->setMargin(0);
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
    addProgressInfo(i18n("Import settings %1", name));
    SieveEditorUtil::addServerSieveConfig(settings);
    mSettingsFound = true;
}
