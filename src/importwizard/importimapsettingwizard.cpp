/*
   Copyright (C) 2016 Montel Laurent <montel@kde.org>

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

#include "importimapsettingwizard.h"
#include "importimapsettingsearchpage.h"
#include "importimapsettingnofoundpage.h"
#include "importimapsettingfinishpage.h"
#include "importimapsettingprogresspage.h"
#include "checks/importimapsettingsakonadicheckjob.h"
#include "checks/importimapsettingsthunderbirdcheckjob.h"

#include <KLocalizedString>
#include <KConfigGroup>
#include <KSharedConfig>

ImportImapSettingWizard::ImportImapSettingWizard(QWidget *parent)
    : KAssistantDialog(parent)
{
    setWindowTitle(i18n("Import IMAP Settings"));

    mSearchPage = new ImportImapSettingSearchPage(this);
    mSearchPageItem = new KPageWidgetItem(mSearchPage, i18n("Select Import Settings"));
    addPage(mSearchPageItem);

    mNoFoundPage = new ImportImapSettingNoFoundPage(this);
    mNoFoundPageItem = new KPageWidgetItem(mNoFoundPage);
    addPage(mNoFoundPageItem);

    mProgressPage = new ImportImapSettingProgressPage(this);
    mProgressPageItem = new KPageWidgetItem(mProgressPage, i18n("Import in Progress..."));
    addPage(mProgressPageItem);

    mFinishPage = new ImportImapSettingFinishPage(this);
    mFinishPageItem = new KPageWidgetItem(mFinishPage, i18n("Finish"));
    addPage(mFinishPageItem);

    readConfig();
    initializeWizard();
}

ImportImapSettingWizard::~ImportImapSettingWizard()
{
    writeConfig();
}

void ImportImapSettingWizard::initializeWizard()
{
    ImportImapSettingsThunderbirdCheckJob *thjob = new ImportImapSettingsThunderbirdCheckJob(this);
    thjob->name();
    mListCheckJob.insert(thjob->name(), thjob);
    ImportImapSettingsAkonadiCheckJob *akonadiJob = new ImportImapSettingsAkonadiCheckJob(this);
    mListCheckJob.insert(akonadiJob->name(), akonadiJob);

    bool hasSettingToImport = false;
    const QList<AbstractImapSettingsCheckJob *> listJob = mListCheckJob.values();
    for (AbstractImapSettingsCheckJob *job : listJob) {
        if (job->settingsCanBeImported()) {
            hasSettingToImport = true;
            break;
        }
    }
    if (hasSettingToImport) {
        setAppropriate(mNoFoundPageItem, false);
        setAppropriate(mSearchPageItem, true);
        setAppropriate(mProgressPageItem, true);
        setAppropriate(mFinishPageItem, true);
        setCurrentPage(mSearchPageItem);
    } else {
        setCurrentPage(mNoFoundPageItem);
        setAppropriate(mNoFoundPageItem, true);
        setAppropriate(mSearchPageItem, false);
        setAppropriate(mProgressPageItem, false);
        setAppropriate(mFinishPageItem, false);
    }
}

void ImportImapSettingWizard::next()
{
    if (currentPage() == mNoFoundPageItem) {
        if (isAppropriate(mSearchPageItem)) {
            mSearchPage->setProgramList(mListCheckJob.keys());
        }
    } else if (currentPage() == mSearchPageItem) {
        const QStringList programs = mSearchPage->selectedPrograms();
        if (programs.isEmpty()) {
            //Go to end
            setAppropriate(mProgressPageItem, false);
        } else {
            mProgressPage->setSelectedPrograms(programs);
        }
    }
    KAssistantDialog::next();
}

void ImportImapSettingWizard::readConfig()
{
    KConfigGroup group(KSharedConfig::openConfig(), "SieveEditorConfigureDialog");
    const QSize size = group.readEntry("Size", QSize(600, 400));
    if (size.isValid()) {
        resize(size);
    }
}

void ImportImapSettingWizard::writeConfig()
{
    KConfigGroup group(KSharedConfig::openConfig(), "SieveEditorConfigureDialog");
    group.writeEntry("Size", size());
    group.sync();
}

