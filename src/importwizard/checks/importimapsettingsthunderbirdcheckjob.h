/*
   SPDX-FileCopyrightText: 2016-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "abstractimapsettingscheckjob.h"
#include "libsieveeditor_private_export.h"
class LIBSIEVEEDITOR_TESTS_EXPORT ImportImapSettingsThunderbirdCheckJob : public AbstractImapSettingsCheckJob
{
    Q_OBJECT
public:
    explicit ImportImapSettingsThunderbirdCheckJob(QObject *parent = nullptr);
    ~ImportImapSettingsThunderbirdCheckJob() override;

    void start() override;
    [[nodiscard]] bool settingsCanBeImported() const override;
    [[nodiscard]] QString name() const override;

private:
    [[nodiscard]] LIBSIEVEEDITOR_NO_EXPORT bool importSettings(const QString &directory, const QString &defaultProfile);
    [[nodiscard]] LIBSIEVEEDITOR_NO_EXPORT QMap<QString, QString> listProfile(QString &currentProfile, const QString &defaultSettingPath);
    [[nodiscard]] LIBSIEVEEDITOR_NO_EXPORT QString defaultPath() const;
    LIBSIEVEEDITOR_NO_EXPORT void insertIntoMap(const QString &line);
    LIBSIEVEEDITOR_NO_EXPORT void addAuth(SieveEditorUtil::SieveServerConfig &config, const QString &accountName);
    LIBSIEVEEDITOR_NO_EXPORT void encryption(SieveEditorUtil::SieveServerConfig &config, const QString &accountName);

    QHash<QString, QVariant> mHashConfig;
};
