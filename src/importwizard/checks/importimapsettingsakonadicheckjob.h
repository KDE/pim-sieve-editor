/*
   SPDX-FileCopyrightText: 2016-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "abstractimapsettingscheckjob.h"
#include "libsieveeditor_private_export.h"
class LIBSIEVEEDITOR_TESTS_EXPORT ImportImapSettingsAkonadiCheckJob : public AbstractImapSettingsCheckJob
{
    Q_OBJECT
public:
    explicit ImportImapSettingsAkonadiCheckJob(QObject *parent = nullptr);
    ~ImportImapSettingsAkonadiCheckJob() override;

    void start() override;

    [[nodiscard]] bool settingsCanBeImported() const override;
    [[nodiscard]] QString name() const override;
    void setImapSettingsPassword(AbstractImapSettingsPassword *passwordImporter) override;

private:
    [[nodiscard]] inline bool resourceCanHaveSieveSupport(const QString &filename) const;
    void loadSieveServerSettings();
    void slotImportNextServerSieveDone(const SieveEditorUtil::SieveServerConfig &config, const QString &filename);
    void importNextServerSieve();
    void importSettings(const QString &directory, const QString &filename);
    QMultiMap<QString, QString> mSieveServerLst;
    QMultiMap<QString, QString>::const_iterator mSieveServerMapIterator;
};
