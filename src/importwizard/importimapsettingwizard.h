/*
   Copyright (C) 2016-2021 Laurent Montel <montel@kde.org>

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

#ifndef IMPORTIMAPSETTINGWIZARD_H
#define IMPORTIMAPSETTINGWIZARD_H

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

#endif // IMPORTIMAPSETTINGWIZARD_H
