/*
   SPDX-FileCopyrightText: 2016-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef ABSTRACTIMAPSETTINGSCHECKJOB_H
#define ABSTRACTIMAPSETTINGSCHECKJOB_H

#include <QObject>
#include "../../sieveeditorutil.h"
#include "libsieveeditor_private_export.h"
class AbstractImapSettingsPassword;
class LIBSIEVEEDITOR_TESTS_EXPORT AbstractImapSettingsCheckJob : public QObject
{
    Q_OBJECT
public:
    explicit AbstractImapSettingsCheckJob(QObject *parent = nullptr);
    ~AbstractImapSettingsCheckJob();
    virtual void start() = 0;

    virtual Q_REQUIRED_RESULT bool settingsCanBeImported() const = 0;
    virtual Q_REQUIRED_RESULT QString name() const = 0;

    void setImapSettingsPassword(AbstractImapSettingsPassword *passwordImporter);

Q_SIGNALS:
    void importSetting(const QString &name, const SieveEditorUtil::SieveServerConfig &settings);
    void noSettingsImported(const QString &name);

protected:
    Q_DISABLE_COPY(AbstractImapSettingsCheckJob)
    void checkNoSettingsImported();
    AbstractImapSettingsPassword *mPasswordImporter = nullptr;
    bool mSettingsWereImported = false;
};

#endif // ABSTRACTIMAPSETTINGSCHECKJOB_H
