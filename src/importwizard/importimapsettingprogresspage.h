/*
   SPDX-FileCopyrightText: 2016-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "libsieveeditor_private_export.h"
#include "sieveeditorutil.h"
#include <QMap>
#include <QWidget>

class QTextEdit;
class AbstractImapSettingsCheckJob;
class LIBSIEVEEDITOR_TESTS_EXPORT ImportImapSettingProgressPage : public QWidget
{
    Q_OBJECT
public:
    explicit ImportImapSettingProgressPage(QWidget *parent = nullptr);
    ~ImportImapSettingProgressPage() override;
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
    QTextEdit *mProgressTextEdit = nullptr;
    QMap<QString, AbstractImapSettingsCheckJob *> mListCheckJob;
    bool mSettingsFound = false;
};

