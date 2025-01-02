/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "libsieveeditor_export.h"
namespace KUserFeedback
{
class Provider;
}
class LIBSIEVEEDITOR_EXPORT UserFeedBackManager
{
public:
    ~UserFeedBackManager();
    static UserFeedBackManager *self();

    KUserFeedback::Provider *userFeedbackProvider() const;

private:
    UserFeedBackManager();
    KUserFeedback::Provider *const mUserFeedbackProvider;
};
