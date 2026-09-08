/*
   SPDX-FileCopyrightText: 2016-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "importimapsettingwizard.h"
#include "checks/importimapsettingsakonadicheckjob.h"
#include "checks/importimapsettingsthunderbirdcheckjob.h"
#include "importimapsettingfinishpage.h"
#include "importimapsettingnofoundpage.h"
#include "importimapsettingprogresspage.h"
#include "importimapsettingsearchpage.h"

#include "importwizard/checks/importimapsettingsakonadipassword.h"
#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <PimCommon/PimUtil>

#include <KWindowConfig>
#include <QPushButton>
#include <QWindow>
#include <TextAddonsWidgets/LoadDialogSizeUtils>

ImportImapSettingWizard::ImportImapSettingWizard(QWidget *parent)
    : KAssistantDialog(parent)
    , mSearchPage(new ImportImapSettingSearchPage(this))
    , mNoFoundPage(new ImportImapSettingNoFoundPage(this))
    , mProgressPage(new ImportImapSettingProgressPage(this))
    , mFinishPage(new ImportImapSettingFinishPage(this))
{
    setWindowTitle(i18nc("@title:window", "Import IMAP Settings"));

    mSearchPageItem = new KPageWidgetItem(mSearchPage, i18n("Select Import Settings"));
    addPage(mSearchPageItem);

    mNoFoundPageItem = new KPageWidgetItem(mNoFoundPage);
    addPage(mNoFoundPageItem);

    mProgressPageItem = new KPageWidgetItem(mProgressPage, i18n("Import in Progress…"));
    connect(mProgressPage, &ImportImapSettingProgressPage::finished, this, &ImportImapSettingWizard::slotFinishImportData);
    connect(mProgressPage, &ImportImapSettingProgressPage::noSettingsImported, this, &ImportImapSettingWizard::slotAddSummaryInfo);
    connect(mProgressPage, &ImportImapSettingProgressPage::addSummaryInfo, this, &ImportImapSettingWizard::slotAddSummaryInfo);
    addPage(mProgressPageItem);

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
    auto thjob = new ImportImapSettingsThunderbirdCheckJob(this);
    if (thjob->settingsCanBeImported()) {
        mListCheckJob.insert(thjob->name(), thjob);
        hasSettingToImport = true;
    } else {
        delete thjob;
    }
    auto akonadiJob = new ImportImapSettingsAkonadiCheckJob(this);
    akonadiJob->setImapSettingsPassword(new ImportImapSettingsAkonadiPassword);
    if (akonadiJob->settingsCanBeImported()) {
        mListCheckJob.insert(akonadiJob->name(), akonadiJob);
        hasSettingToImport = true;
    } else {
        delete akonadiJob;
    }

    if (hasSettingToImport) {
        // Initialize list
        mSearchPage->setProgramList(mListCheckJob.keys());
        setAppropriate(mNoFoundPageItem, false);
        setAppropriate(mSearchPageItem, false);
        setAppropriate(mProgressPageItem, false);
        setAppropriate(mFinishPageItem, false);
        setCurrentPage(mSearchPageItem);
    } else {
        setAppropriate(mNoFoundPageItem, true);
        setAppropriate(mSearchPageItem, false);
        setAppropriate(mProgressPageItem, false);
        setAppropriate(mFinishPageItem, false);
        setCurrentPage(mNoFoundPageItem);
    }
}

void ImportImapSettingWizard::slotNeedToImportSettings(bool b)
{
    setAppropriate(mSearchPageItem, b);
    setAppropriate(mProgressPageItem, b);
    setAppropriate(mFinishPageItem, b);
}

void ImportImapSettingWizard::next()
{
    if (currentPage() == mSearchPageItem) {
        const QStringList programs = mSearchPage->selectedPrograms();
        if (programs.isEmpty()) {
            // Go to end
            setAppropriate(mProgressPageItem, false);
        } else {
            mProgressPage->setListCheckJob(mListCheckJob);
            mProgressPage->setSelectedPrograms(programs);
            // Start importation
            mProgressPage->start();
        }
    }
    KAssistantDialog::next();
}

void ImportImapSettingWizard::readConfig()
{
#if TEXTADDONSWIDGETS_VERSION >= QT_VERSION_CHECK(2, 1, 49)
    TextAddonsWidgets::LoadDialogSizeUtils::manageDialogSize(this, QStringLiteral("ImportImapSettingWizard"), QSize(600, 400));
#else
    create(); // ensure a window is created
    windowHandle()->resize(QSize(600, 400));
    const KConfigGroup group(KSharedConfig::openStateConfig(), QStringLiteral("ImportImapSettingWizard"));
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
#endif
}

void ImportImapSettingWizard::writeConfig()
{
#if TEXTADDONSWIDGETS_VERSION < QT_VERSION_CHECK(2, 1, 49)
    KConfigGroup group(KSharedConfig::openStateConfig(), QStringLiteral("ImportImapSettingWizard"));
    KWindowConfig::saveWindowSize(windowHandle(), group);
    group.sync();
#endif
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

#include "moc_importimapsettingwizard.cpp"
