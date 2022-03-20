/*
   SPDX-FileCopyrightText: 2016-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "libsieveeditor_export.h"
#include <KAssistantDialog>

class ImportImapSettingSearchPage;
class ImportImapSettingNoFoundPage;
class ImportImapSettingProgressPage;
class ImportImapSettingFinishPage;
class KPageWidgetItem;
class AbstractImapSettingsCheckJob;
class LIBSIEVEEDITOR_EXPORT ImportImapSettingWizard : public KAssistantDialog
{
    Q_OBJECT
public:
    explicit ImportImapSettingWizard(QWidget *parent = nullptr);
    ~ImportImapSettingWizard() override;

    void next() override;

private:
    void readConfig();
    void writeConfig();
    void initializeWizard();
    void slotHelpClicked();
    void slotFinishImportData();
    void slotAddSummaryInfo(const QString &name);
    void slotNeedToImportSettings(bool b);
    QMap<QString, AbstractImapSettingsCheckJob *> mListCheckJob;

    ImportImapSettingSearchPage *mSearchPage = nullptr;
    KPageWidgetItem *mSearchPageItem = nullptr;

    ImportImapSettingNoFoundPage *mNoFoundPage = nullptr;
    KPageWidgetItem *mNoFoundPageItem = nullptr;

    ImportImapSettingProgressPage *mProgressPage = nullptr;
    KPageWidgetItem *mProgressPageItem = nullptr;

    ImportImapSettingFinishPage *mFinishPage = nullptr;
    KPageWidgetItem *mFinishPageItem = nullptr;
};
