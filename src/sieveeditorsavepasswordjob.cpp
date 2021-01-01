/*
   Copyright (C) 2020-2021 Laurent Montel <montel@kde.org>

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

#include "sieveeditorsavepasswordjob.h"
#include "sieveeditor_debug.h"
#include <qt5keychain/keychain.h>

using namespace QKeychain;

SieveEditorSavePasswordJob::SieveEditorSavePasswordJob(QObject *parent)
    : QObject(parent)
{
}

SieveEditorSavePasswordJob::~SieveEditorSavePasswordJob()
{
}

bool SieveEditorSavePasswordJob::canStart() const
{
    return !mName.isEmpty() && !mPassword.isEmpty() && !mKey.isEmpty();
}

void SieveEditorSavePasswordJob::start()
{
    if (!canStart()) {
        if (mName.isEmpty()) {
            qCWarning(SIEVEEDITOR_LOG) << "Error missing mName";
        }
        if (mKey.isEmpty()) {
            qCWarning(SIEVEEDITOR_LOG) << "Error missing mKey";
        }
        if (mPassword.isEmpty()) {
            qCWarning(SIEVEEDITOR_LOG) << "Error missing mName";
        }
        deleteLater();
        return;
    }
    auto writeJob = new WritePasswordJob(mName, this);
    connect(writeJob, &Job::finished, this, &SieveEditorSavePasswordJob::slotPasswordWritten);
    writeJob->setKey(mKey);
    writeJob->setTextData(mPassword);
    writeJob->start();
}

void SieveEditorSavePasswordJob::slotPasswordWritten(QKeychain::Job *baseJob)
{
    if (baseJob->error()) {
        qCWarning(SIEVEEDITOR_LOG) << "Error writing password using QKeychain:" << baseJob->errorString();
    }
    deleteLater();
}

QString SieveEditorSavePasswordJob::name() const
{
    return mName;
}

void SieveEditorSavePasswordJob::setName(const QString &name)
{
    mName = name;
}

QString SieveEditorSavePasswordJob::key() const
{
    return mKey;
}

void SieveEditorSavePasswordJob::setKey(const QString &key)
{
    mKey = key;
}

QString SieveEditorSavePasswordJob::password() const
{
    return mPassword;
}

void SieveEditorSavePasswordJob::setPassword(const QString &password)
{
    mPassword = password;
}
