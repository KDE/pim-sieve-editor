/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "libsieveeditor_export.h"
#include <QObject>
namespace QKeychain
{
class Job;
}
class LIBSIEVEEDITOR_EXPORT SieveEditorSavePasswordJob : public QObject
{
    Q_OBJECT
public:
    explicit SieveEditorSavePasswordJob(QObject *parent = nullptr);
    ~SieveEditorSavePasswordJob() override;

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
