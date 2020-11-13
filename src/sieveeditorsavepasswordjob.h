/*
   Copyright (C) 2020 Laurent Montel <montel@kde.org>

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

#ifndef SIEVEEDITORSAVEPASSWORDJOB_H
#define SIEVEEDITORSAVEPASSWORDJOB_H

#include <QObject>
#include "libsieveeditor_export.h"
#include <qt5keychain/keychain.h>
class LIBSIEVEEDITOR_EXPORT SieveEditorSavePasswordJob : public QObject
{
    Q_OBJECT
public:
    explicit SieveEditorSavePasswordJob(QObject *parent = nullptr);
    ~SieveEditorSavePasswordJob();

    Q_REQUIRED_RESULT bool canStart() const;

    void start();

    Q_REQUIRED_RESULT QString password() const;
    void setPassword(const QString &password);

    Q_REQUIRED_RESULT QString key() const;
    void setKey(const QString &key);

    Q_REQUIRED_RESULT QString name() const;
    void setName(const QString &name);

private:
    void slotPasswordWritten(QKeychain::Job *baseJob);
    QString mPassword;
    QString mKey;
    QString mName;
};

#endif // SIEVEEDITORSAVEPASSWORDJOB_H
