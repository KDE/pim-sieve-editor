/*
   SPDX-FileCopyrightText: 2016-2023 Laurent Montel <montel@kde.org>

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
    [[nodiscard]] bool importSettings(const QString &directory, const QString &defaultProfile);
    [[nodiscard]] QMap<QString, QString> listProfile(QString &currentProfile, const QString &defaultSettingPath);
    [[nodiscard]] QString defaultPath() const;
    void insertIntoMap(const QString &line);
    void addAuth(SieveEditorUtil::SieveServerConfig &config, const QString &accountName);
    void encryption(SieveEditorUtil::SieveServerConfig &config, const QString &accountName);

    QHash<QString, QVariant> mHashConfig;
};
