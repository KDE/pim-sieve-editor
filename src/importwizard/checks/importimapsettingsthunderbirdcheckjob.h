/*
   Copyright (C) 2016-2017 Montel Laurent <montel@kde.org>

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

#ifndef IMPORTIMAPSETTINGSTHUNDERBIRDCHECKJOB_H
#define IMPORTIMAPSETTINGSTHUNDERBIRDCHECKJOB_H

#include "abstractimapsettingscheckjob.h"
#include "libsieveeditor_private_export.h"
class LIBSIEVEEDITOR_TESTS_EXPORT ImportImapSettingsThunderbirdCheckJob : public AbstractImapSettingsCheckJob
{
    Q_OBJECT
public:
    explicit ImportImapSettingsThunderbirdCheckJob(QObject *parent = nullptr);
    ~ImportImapSettingsThunderbirdCheckJob();

    void start() Q_DECL_OVERRIDE;
    bool settingsCanBeImported() const Q_DECL_OVERRIDE;
    QString name() const Q_DECL_OVERRIDE;
private:
    bool importSettings(const QString &directory, const QString &defaultProfile);
    QMap<QString, QString> listProfile(QString &currentProfile, const QString &defaultSettingPath);
    QString defaultPath() const;
    void insertIntoMap(const QString &line);
    void addAuth(QMap<QString, QVariant> &settings, const QString &argument, const QString &accountName);

    QHash<QString, QVariant> mHashConfig;
};

#endif // IMPORTIMAPSETTINGSTHUNDERBIRDCHECKJOB_H
