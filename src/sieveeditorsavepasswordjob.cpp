/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "sieveeditorsavepasswordjob.h"
#include "sieveeditor_debug.h"
#include <qt6keychain/keychain.h>

using namespace QKeychain;

SieveEditorSavePasswordJob::SieveEditorSavePasswordJob(QObject *parent)
    : QObject(parent)
{
}

SieveEditorSavePasswordJob::~SieveEditorSavePasswordJob() = default;

bool SieveEditorSavePasswordJob::canStart() const
{
    return !mName.isEmpty() && !mPassword.isEmpty() && !mKey.isEmpty();
}

void SieveEditorSavePasswordJob::start()
{
    if (!canStart()) {
        if (mName.isEmpty()) {
            qCWarning(SIEVEEDITOR_LOG) << "Error missing name info";
        }
        if (mKey.isEmpty()) {
            qCWarning(SIEVEEDITOR_LOG) << "Error missing key";
        }
        if (mPassword.isEmpty()) {
            qCWarning(SIEVEEDITOR_LOG) << "Error missing password";
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
