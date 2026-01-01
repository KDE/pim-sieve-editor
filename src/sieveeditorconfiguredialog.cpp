/*
   SPDX-FileCopyrightText: 2013-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "sieveeditorconfiguredialog.h"
using namespace Qt::Literals::StringLiterals;

#include "sieveeditorconfigureserverwidget.h"
#include "sieveeditorglobalconfig.h"

#if WITH_KUSERFEEDBACK
#include "userfeedback/userfeedbackmanager.h"
#include <KUserFeedback/FeedbackConfigWidget>
#include <KUserFeedback/Provider>
#endif

#include <KConfigDialogManager>
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
    , mWrapText(new QCheckBox(i18nc("@option:check", "Wrap Text"), this))
    , m_configDialogManager(new KConfigDialogManager(this, SieveEditorGlobalConfig::self()))
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
    editorWidget->setObjectName("editorwidget"_L1);

    layout = new QVBoxLayout(editorWidget);
    mWrapText->setObjectName("kcfg_WrapText"_L1);
    layout->addWidget(mWrapText);
    layout->addStretch(100);
    m_configDialogManager->addWidget(editorWidget);

    auto editorPageWidgetPage = new KPageWidgetItem(editorWidget, i18n("Editor"));
    editorPageWidgetPage->setIcon(QIcon::fromTheme(QStringLiteral("accessories-text-editor")));
    addPage(editorPageWidgetPage);

    // UserFeedBack config
#if WITH_KUSERFEEDBACK
    auto userFeedBackWidget = new QWidget;
    userFeedBackWidget->setObjectName("userFeedBackWidget"_L1);

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
    m_configDialogManager->updateWidgets();
}

void SieveEditorConfigureDialog::saveServerSieveConfig()
{
    mServerWidget->writeConfig();
    m_configDialogManager->updateSettings();
    SieveEditorGlobalConfig::self()->save();
#if WITH_KUSERFEEDBACK
    // set current active mode + write back the config for future starts
    UserFeedBackManager::self()->userFeedbackProvider()->setTelemetryMode(mUserFeedbackWidget->telemetryMode());
    UserFeedBackManager::self()->userFeedbackProvider()->setSurveyInterval(mUserFeedbackWidget->surveyInterval());
#endif
}

void SieveEditorConfigureDialog::readConfig()
{
    create(); // ensure a window is created
    windowHandle()->resize(QSize(600, 400));
    const KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(mySieveEditorConfigureDialog));
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void SieveEditorConfigureDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(mySieveEditorConfigureDialog));
    KWindowConfig::saveWindowSize(windowHandle(), group);
    group.sync();
}

#include "moc_sieveeditorconfiguredialog.cpp"
