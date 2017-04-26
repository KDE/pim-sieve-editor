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

#ifndef IMPORTIMAPSETTINGPROGRESSPAGE_H
#define IMPORTIMAPSETTINGPROGRESSPAGE_H

#include <QWidget>
#include <QMap>
#include "sieveeditorutil.h"
#include "libsieveeditor_private_export.h"

class QTextEdit;
class AbstractImapSettingsCheckJob;
class LIBSIEVEEDITOR_TESTS_EXPORT ImportImapSettingProgressPage : public QWidget
{
    Q_OBJECT
public:
    explicit ImportImapSettingProgressPage(QWidget *parent = nullptr);
    ~ImportImapSettingProgressPage();
    void addProgressInfo(const QString &str);
    void setSelectedPrograms(const QStringList &programs);
    void setListCheckJob(const QMap<QString, AbstractImapSettingsCheckJob *> &listCheckJob);

    void start();

Q_SIGNALS:
    void addSummaryInfo(const QString &info);
    void finished();
    void noSettingsImported(const QString &name);

private:
    void slotImportSettingsDone(const QString &name, const SieveEditorUtil::SieveServerConfig &settings);
    QStringList mSelectedPrograms;
    QTextEdit *mProgressTextEdit;
    QMap<QString, AbstractImapSettingsCheckJob *> mListCheckJob;
    bool mSettingsFound;
};

#endif // IMPORTIMAPSETTINGPROGRESSPAGE_H
