/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

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

    [[nodiscard]] bool canStart() const;

    void start();

    [[nodiscard]] QString password() const;
    void setPassword(const QString &password);

    [[nodiscard]] QString key() const;
    void setKey(const QString &key);

    [[nodiscard]] QString name() const;
    void setName(const QString &name);

private:
    void slotPasswordWritten(QKeychain::Job *baseJob);
    QString mPassword;
    QString mKey;
    QString mName;
};
