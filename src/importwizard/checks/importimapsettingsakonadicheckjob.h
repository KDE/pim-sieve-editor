/*
   Copyright (C) 2016-2021 Laurent Montel <montel@kde.org>

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

    Q_REQUIRED_RESULT bool settingsCanBeImported() const override;
    Q_REQUIRED_RESULT QString name() const override;
    void setImapSettingsPassword(AbstractImapSettingsPassword *passwordImporter) override;

private:
    inline Q_REQUIRED_RESULT bool resourceCanHaveSieveSupport(const QString &filename) const;
    void loadSieveServerSettings();
    void slotImportNextServerSieveDone(const SieveEditorUtil::SieveServerConfig &config, const QString &filename);
    void importNextServerSieve();
    void importSettings(const QString &directory, const QString &filename);
    QMultiMap<QString, QString> mSieveServerLst;
    QMap<QString, QString>::const_iterator mSieveServerMapIterator;
};

