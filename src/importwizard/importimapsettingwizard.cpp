/*
   Copyright (C) 2016-2018 Montel Laurent <montel@kde.org>

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
#include <PimCommon/PimUtil>
#include <importwizard/checks/importimapsettingsakonadipassword.h>

#include <QPushButton>

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
    connect(mProgressPage, &ImportImapSettingProgressPage::finished, this, &ImportImapSettingWizard::slotFinishImportData);
    connect(mProgressPage, &ImportImapSettingProgressPage::noSettingsImported, this, &ImportImapSettingWizard::slotAddSummaryInfo);
    connect(mProgressPage, &ImportImapSettingProgressPage::addSummaryInfo, this, &ImportImapSettingWizard::slotAddSummaryInfo);
    addPage(mProgressPageItem);

    mFinishPage = new ImportImapSettingFinishPage(this);
    mFinishPageItem = new KPageWidgetItem(mFinishPage, i18n("Finish"));
    addPage(mFinishPageItem);

    readConfig();
    initializeWizard();
    connect(button(QDialogButtonBox::Help), &QPushButton::clicked, this, &ImportImapSettingWizard::slotHelpClicked);
    connect(mSearchPage, &ImportImapSettingSearchPage::needToImportSettings, this, &ImportImapSettingWizard::slotNeedToImportSettings);
}

ImportImapSettingWizard::~ImportImapSettingWizard()
{
    writeConfig();
}

void ImportImapSettingWizard::initializeWizard()
{
    bool hasSettingToImport = false;
    ImportImapSettingsThunderbirdCheckJob *thjob = new ImportImapSettingsThunderbirdCheckJob(this);
    if (thjob->settingsCanBeImported()) {
        mListCheckJob.insert(thjob->name(), thjob);
        hasSettingToImport = true;
    } else {
        delete thjob;
    }
    ImportImapSettingsAkonadiCheckJob *akonadiJob = new ImportImapSettingsAkonadiCheckJob(this);
    akonadiJob->setImapSettingsPassword(new ImportImapSettingsAkonadiPassword);
    if (akonadiJob->settingsCanBeImported()) {
        mListCheckJob.insert(akonadiJob->name(), akonadiJob);
        hasSettingToImport = true;
    } else {
        delete akonadiJob;
    }

    if (hasSettingToImport) {
        //Initialize list
        mSearchPage->setProgramList(mListCheckJob.keys());
        setAppropriate(mNoFoundPageItem, false);
        setAppropriate(mSearchPageItem, true);
        setAppropriate(mProgressPageItem, true);
        setAppropriate(mFinishPageItem, true);
        setCurrentPage(mSearchPageItem);
    } else {
        setAppropriate(mNoFoundPageItem, true);
        setAppropriate(mSearchPageItem, false);
        setAppropriate(mProgressPageItem, false);
        setAppropriate(mFinishPageItem, false);
        setCurrentPage(mNoFoundPageItem);
    }
    nextButton()->setEnabled(false);
}

void ImportImapSettingWizard::slotNeedToImportSettings(bool b)
{
    nextButton()->setEnabled(b);
}

void ImportImapSettingWizard::next()
{
    if (currentPage() == mSearchPageItem) {
        const QStringList programs = mSearchPage->selectedPrograms();
        if (programs.isEmpty()) {
            //Go to end
            setAppropriate(mProgressPageItem, false);
        } else {
            mProgressPage->setListCheckJob(mListCheckJob);
            mProgressPage->setSelectedPrograms(programs);
            //Start importation
            mProgressPage->start();
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

void ImportImapSettingWizard::slotFinishImportData()
{
    setAppropriate(mFinishPageItem, true);
    setAppropriate(mSearchPageItem, false);
}

void ImportImapSettingWizard::slotAddSummaryInfo(const QString &name)
{
    mFinishPage->addSummary(name);
}

void ImportImapSettingWizard::slotHelpClicked()
{
    PimCommon::Util::invokeHelp(QStringLiteral("sieveeditor/index.html"));
}
