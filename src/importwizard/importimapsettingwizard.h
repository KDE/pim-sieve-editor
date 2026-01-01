/*
   SPDX-FileCopyrightText: 2016-2026 Laurent Montel <montel@kde.org>

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
    LIBSIEVEEDITOR_NO_EXPORT void readConfig();
    LIBSIEVEEDITOR_NO_EXPORT void writeConfig();
    LIBSIEVEEDITOR_NO_EXPORT void initializeWizard();
    LIBSIEVEEDITOR_NO_EXPORT void slotHelpClicked();
    LIBSIEVEEDITOR_NO_EXPORT void slotFinishImportData();
    LIBSIEVEEDITOR_NO_EXPORT void slotAddSummaryInfo(const QString &name);
    LIBSIEVEEDITOR_NO_EXPORT void slotNeedToImportSettings(bool b);
    QMap<QString, AbstractImapSettingsCheckJob *> mListCheckJob;

    ImportImapSettingSearchPage *const mSearchPage;
    KPageWidgetItem *mSearchPageItem = nullptr;

    ImportImapSettingNoFoundPage *const mNoFoundPage;
    KPageWidgetItem *mNoFoundPageItem = nullptr;

    ImportImapSettingProgressPage *const mProgressPage;
    KPageWidgetItem *mProgressPageItem = nullptr;

    ImportImapSettingFinishPage *const mFinishPage;
    KPageWidgetItem *mFinishPageItem = nullptr;
};
