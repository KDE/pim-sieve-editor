/*
   SPDX-FileCopyrightText: 2016-2025 Laurent Montel <montel@kde.org>

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
    [[nodiscard]] LIBSIEVEEDITOR_NO_EXPORT inline bool resourceCanHaveSieveSupport(const QString &filename) const;
    LIBSIEVEEDITOR_NO_EXPORT void loadSieveServerSettings();
    LIBSIEVEEDITOR_NO_EXPORT void slotImportNextServerSieveDone(const SieveEditorUtil::SieveServerConfig &config, const QString &filename);
    LIBSIEVEEDITOR_NO_EXPORT void importNextServerSieve();
    LIBSIEVEEDITOR_NO_EXPORT void importSettings(const QString &directory, const QString &filename);
    QMultiMap<QString, QString> mSieveServerLst;
    QMultiMap<QString, QString>::const_iterator mSieveServerMapIterator;
};
