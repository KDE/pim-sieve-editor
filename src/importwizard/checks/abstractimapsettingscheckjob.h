/*
   SPDX-FileCopyrightText: 2016-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "../../sieveeditorutil.h"
#include "libsieveeditor_private_export.h"
#include <QObject>
class AbstractImapSettingsPassword;
class LIBSIEVEEDITOR_TESTS_EXPORT AbstractImapSettingsCheckJob : public QObject
{
    Q_OBJECT
public:
    explicit AbstractImapSettingsCheckJob(QObject *parent = nullptr);
    ~AbstractImapSettingsCheckJob() override;
    virtual void start() = 0;

    [[nodiscard]] virtual bool settingsCanBeImported() const = 0;
    [[nodiscard]] virtual QString name() const = 0;

    virtual void setImapSettingsPassword(AbstractImapSettingsPassword *passwordImporter);

Q_SIGNALS:
    void importSetting(const QString &name, const SieveEditorUtil::SieveServerConfig &settings);
    void noSettingsImported(const QString &name);

protected:
    Q_DISABLE_COPY(AbstractImapSettingsCheckJob)
    LIBSIEVEEDITOR_NO_EXPORT void checkNoSettingsImported();
    AbstractImapSettingsPassword *mPasswordImporter = nullptr;
    bool mSettingsWereImported = false;
};
