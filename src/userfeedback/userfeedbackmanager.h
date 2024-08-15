/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "libsieveeditor_export.h"
#include <QObject>
namespace KUserFeedback
{
class Provider;
}
class LIBSIEVEEDITOR_EXPORT UserFeedBackManager : public QObject
{
    Q_OBJECT
public:
    static UserFeedBackManager *self();

    KUserFeedback::Provider *userFeedbackProvider() const;

private:
    explicit UserFeedBackManager(QObject *parent = nullptr);
    KUserFeedback::Provider *const mUserFeedbackProvider;
};
