/*
   Copyright (C) 2017-2021 Laurent Montel <montel@kde.org>

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

#include <sieveeditorutil.h>

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

