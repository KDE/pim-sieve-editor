/*
   SPDX-FileCopyrightText: 2013-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "sieveeditorconfiguredialog.h"
#include "sieveeditorconfigureserverwidget.h"
#include "sieveeditorglobalconfig.h"
#include <PimCommon/ConfigureImmutableWidgetUtils>

#ifdef WITH_KUSERFEEDBACK
#include "userfeedback/userfeedbackmanager.h"
#include <KUserFeedback/FeedbackConfigWidget>
#include <KUserFeedback/Provider>
#endif

#include <KLocalizedString>
#include <KSharedConfig>

#include <KConfigGroup>
#include <KWindowConfig>
#include <QCheckBox>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWindow>

namespace
{
static const char mySieveEditorConfigureDialog[] = "SieveEditorConfigureDialog";
}

SieveEditorConfigureDialog::SieveEditorConfigureDialog(QWidget *parent)
    : KPageDialog(parent)
{
    setWindowTitle(i18nc("@title:window", "Configure"));
    setFaceType(KPageDialog::List);

    buttonBox()->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    connect(buttonBox(), &QDialogButtonBox::accepted, this, &SieveEditorConfigureDialog::accept);
    connect(buttonBox(), &QDialogButtonBox::rejected, this, &SieveEditorConfigureDialog::reject);

    // Server page
    auto serverConfigureWiget = new QWidget;
    auto layout = new QVBoxLayout(serverConfigureWiget);
    mServerWidget = new SieveEditorConfigureServerWidget;
    layout->addWidget(mServerWidget);

    auto serverPageWidgetPage = new KPageWidgetItem(serverConfigureWiget, i18n("Sieve Server"));
    serverPageWidgetPage->setIcon(QIcon::fromTheme(QStringLiteral("network-workgroup")));
    addPage(serverPageWidgetPage);

    // Editor page
    auto editorWidget = new QWidget;
    editorWidget->setObjectName(QStringLiteral("editorwidget"));

    layout = new QVBoxLayout(editorWidget);
    mWrapText = new QCheckBox(i18n("Wrap Text"));
    mWrapText->setObjectName(QStringLiteral("wraptext"));
    layout->addWidget(mWrapText);
    layout->addStretch(100);

    auto editorPageWidgetPage = new KPageWidgetItem(editorWidget, i18n("Editor"));
    editorPageWidgetPage->setIcon(QIcon::fromTheme(QStringLiteral("accessories-text-editor")));
    addPage(editorPageWidgetPage);

    // UserFeedBack config
#ifdef WITH_KUSERFEEDBACK
    auto userFeedBackWidget = new QWidget;
    userFeedBackWidget->setObjectName(QStringLiteral("userFeedBackWidget"));

    mUserFeedbackWidget = new KUserFeedback::FeedbackConfigWidget(this);

    auto userFeedBackLayout = new QHBoxLayout(userFeedBackWidget);
    userFeedBackLayout->setContentsMargins({});
    userFeedBackLayout->addWidget(mUserFeedbackWidget);

    mUserFeedbackWidget->setFeedbackProvider(UserFeedBackManager::self()->userFeedbackProvider());
    auto userFeedBackPageWidgetPage = new KPageWidgetItem(userFeedBackWidget, i18n("User Feedback"));
    userFeedBackPageWidgetPage->setIcon(QIcon::fromTheme(QStringLiteral("preferences-other")));
    addPage(userFeedBackPageWidgetPage);
#endif

    loadServerSieveConfig();
    readConfig();
}

SieveEditorConfigureDialog::~SieveEditorConfigureDialog()
{
    writeConfig();
}

void SieveEditorConfigureDialog::loadServerSieveConfig()
{
    mServerWidget->readConfig();
    PimCommon::ConfigureImmutableWidgetUtils::loadWidget(mWrapText, SieveEditorGlobalConfig::self()->wrapTextItem());
}

void SieveEditorConfigureDialog::saveServerSieveConfig()
{
    mServerWidget->writeConfig();
    PimCommon::ConfigureImmutableWidgetUtils::saveCheckBox(mWrapText, SieveEditorGlobalConfig::self()->wrapTextItem());
    SieveEditorGlobalConfig::self()->save();
#ifdef WITH_KUSERFEEDBACK
    // set current active mode + write back the config for future starts
    UserFeedBackManager::self()->userFeedbackProvider()->setTelemetryMode(mUserFeedbackWidget->telemetryMode());
    UserFeedBackManager::self()->userFeedbackProvider()->setSurveyInterval(mUserFeedbackWidget->surveyInterval());
#endif
}

void SieveEditorConfigureDialog::readConfig()
{
    create(); // ensure a window is created
    windowHandle()->resize(QSize(600, 400));
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1String(mySieveEditorConfigureDialog));
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void SieveEditorConfigureDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1String(mySieveEditorConfigureDialog));
    KWindowConfig::saveWindowSize(windowHandle(), group);
    group.sync();
}

#include "moc_sieveeditorconfiguredialog.cpp"
