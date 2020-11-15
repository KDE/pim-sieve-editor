/*
   Copyright (C) 2016-2020 Laurent Montel <montel@kde.org>

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
    ~AbstractImapSettingsCheckJob() override;
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
