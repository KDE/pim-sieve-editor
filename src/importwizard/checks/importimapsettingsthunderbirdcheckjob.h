/*
   SPDX-FileCopyrightText: 2016-2021 Laurent Montel <montel@kde.org>

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
    Q_REQUIRED_RESULT bool settingsCanBeImported() const override;
    Q_REQUIRED_RESULT QString name() const override;

private:
    Q_REQUIRED_RESULT bool importSettings(const QString &directory, const QString &defaultProfile);
    Q_REQUIRED_RESULT QMap<QString, QString> listProfile(QString &currentProfile, const QString &defaultSettingPath);
    Q_REQUIRED_RESULT QString defaultPath() const;
    void insertIntoMap(const QString &line);
    void addAuth(SieveEditorUtil::SieveServerConfig &config, const QString &accountName);
    void encryption(SieveEditorUtil::SieveServerConfig &config, const QString &accountName);

    QHash<QString, QVariant> mHashConfig;
};

