/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "userfeedbackmanager.h"
#include <KUserFeedback/ApplicationVersionSource>
#include <KUserFeedback/LocaleInfoSource>
#include <KUserFeedback/PlatformInfoSource>
#include <KUserFeedback/Provider>
#include <KUserFeedback/QtVersionSource>
#include <KUserFeedback/ScreenInfoSource>
#include <KUserFeedback/StartCountSource>
#include <KUserFeedback/UsageTimeSource>

UserFeedBackManager::UserFeedBackManager()
    : mUserFeedbackProvider(new KUserFeedback::Provider())
{
    mUserFeedbackProvider->setProductIdentifier(QStringLiteral("org.kde.pim-sieve-editor"));
    mUserFeedbackProvider->setFeedbackServer(QUrl(QStringLiteral("https://telemetry.kde.org/")));
    mUserFeedbackProvider->setSubmissionInterval(7);
    mUserFeedbackProvider->setApplicationStartsUntilEncouragement(5);
    mUserFeedbackProvider->setEncouragementDelay(30);

    mUserFeedbackProvider->addDataSource(new KUserFeedback::ApplicationVersionSource);
    mUserFeedbackProvider->addDataSource(new KUserFeedback::PlatformInfoSource);
    mUserFeedbackProvider->addDataSource(new KUserFeedback::ScreenInfoSource);
    mUserFeedbackProvider->addDataSource(new KUserFeedback::QtVersionSource);

    mUserFeedbackProvider->addDataSource(new KUserFeedback::StartCountSource);
    mUserFeedbackProvider->addDataSource(new KUserFeedback::UsageTimeSource);

    mUserFeedbackProvider->addDataSource(new KUserFeedback::LocaleInfoSource);
}

UserFeedBackManager *UserFeedBackManager::self()
{
    static UserFeedBackManager s_self;
    return &s_self;
}

KUserFeedback::Provider *UserFeedBackManager::userFeedbackProvider() const
{
    return mUserFeedbackProvider;
}

UserFeedBackManager::~UserFeedBackManager()
{
    delete mUserFeedbackProvider;
}
