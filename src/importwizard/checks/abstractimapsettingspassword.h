/*
   SPDX-FileCopyrightText: 2017-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "sieveeditorutil.h"

#include "libsieveeditor_private_export.h"
class LIBSIEVEEDITOR_TESTS_EXPORT AbstractImapSettingsPassword : public QObject
{
    Q_OBJECT
public:
    explicit AbstractImapSettingsPassword(QObject *parent = nullptr);
    ~AbstractImapSettingsPassword() override;
    virtual void importPasswords(const SieveEditorUtil::SieveServerConfig &config, const QString &filename, bool reuseImapSettings);
Q_SIGNALS:
    void importPasswordDone(const SieveEditorUtil::SieveServerConfig &config, const QString &filename);

protected:
    QString mFileName;
};
