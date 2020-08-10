/*
   SPDX-FileCopyrightText: 2017-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef ABSTRACTIMAPSETTINGSPASSWORD_H
#define ABSTRACTIMAPSETTINGSPASSWORD_H

#include <sieveeditorutil.h>

#include "libsieveeditor_private_export.h"
class LIBSIEVEEDITOR_TESTS_EXPORT AbstractImapSettingsPassword
{
public:
    AbstractImapSettingsPassword();
    virtual ~AbstractImapSettingsPassword();
    virtual void importPasswords(SieveEditorUtil::SieveServerConfig &config, const QString &filename, bool reuseImapSettings);
};

#endif // ABSTRACTIMAPSETTINGSPASSWORD_H
