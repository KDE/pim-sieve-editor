/*
   SPDX-FileCopyrightText: 2016-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef IMPORTIMAPSETTINGSAKONADICHECKJOB_H
#define IMPORTIMAPSETTINGSAKONADICHECKJOB_H

#include "abstractimapsettingscheckjob.h"
#include "libsieveeditor_private_export.h"
class LIBSIEVEEDITOR_TESTS_EXPORT ImportImapSettingsAkonadiCheckJob : public AbstractImapSettingsCheckJob
{
    Q_OBJECT
public:
    explicit ImportImapSettingsAkonadiCheckJob(QObject *parent = nullptr);
    ~ImportImapSettingsAkonadiCheckJob() override;

    void start() override;

    Q_REQUIRED_RESULT bool settingsCanBeImported() const override;
    Q_REQUIRED_RESULT QString name() const override;

private:
    inline bool resourceCanHaveSieveSupport(const QString &filename) const;
    bool importSettings(const QString &directory, const QString &filename);
};

#endif // IMPORTIMAPSETTINGSAKONADICHECKJOB_H
