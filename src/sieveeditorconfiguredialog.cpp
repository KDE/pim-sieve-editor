/*
   Copyright (C) 2013-2020 Laurent Montel <montel@kde.org>

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

#include "sieveeditorconfiguredialog.h"
#include "serversievelistwidget.h"
#include "sieveeditorconfigureserverwidget.h"
#include <PimCommon/ConfigureImmutableWidgetUtils>
#include "sieveeditorglobalconfig.h"

#ifdef WITH_KUSERFEEDBACK
#include "userfeedback/userfeedbackmanager.h"
#include <KUserFeedback/Provider>
#include <KUserFeedback/FeedbackConfigWidget>
#endif

#include <KLocalizedString>
#include <KSharedConfig>

#include <QVBoxLayout>
#include <QCheckBox>
#include <KConfigGroup>
#include <QDialogButtonBox>
#include <QPushButton>

SieveEditorConfigureDialog::SieveEditorConfigureDialog(QWidget *parent)
    : KPageDialog(parent)
{
    setWindowTitle(i18nc("@title:window", "Configure"));
    setFaceType(KPageDialog::List);

    buttonBox()->setStandardButtons(QDialogButtonBox::Ok| QDialogButtonBox::Cancel);

    connect(buttonBox(), &QDialogButtonBox::accepted, this, &SieveEditorConfigureDialog::accept);
    connect(buttonBox(), &QDialogButtonBox::rejected, this, &SieveEditorConfigureDialog::reject);

    //Server page
    QWidget *serverConfigureWiget = new QWidget;
    auto *layout = new QVBoxLayout(serverConfigureWiget);
    mServerWidget = new SieveEditorConfigureServerWidget;
    layout->addWidget(mServerWidget);

    mCloseWallet = new QCheckBox(i18n("Close wallet when close application"));
    layout->addWidget(mCloseWallet);

    KPageWidgetItem *serverPageWidgetPage = new KPageWidgetItem(serverConfigureWiget, i18n("Sieve Server"));
    serverPageWidgetPage->setIcon(QIcon::fromTheme(QStringLiteral("network-workgroup")));
    addPage(serverPageWidgetPage);

    //Editor page
    QWidget *editorWidget = new QWidget;
    editorWidget->setObjectName(QStringLiteral("editorwidget"));

    layout = new QVBoxLayout(editorWidget);
    mWrapText = new QCheckBox(i18n("Wrap Text"));
    mWrapText->setObjectName(QStringLiteral("wraptext"));
    layout->addWidget(mWrapText);
    layout->addStretch(100);

    KPageWidgetItem *editorPageWidgetPage = new KPageWidgetItem(editorWidget, i18n("Editor"));
    editorPageWidgetPage->setIcon(QIcon::fromTheme(QStringLiteral("accessories-text-editor")));
    addPage(editorPageWidgetPage);

    //UserFeedBack config
#ifdef WITH_KUSERFEEDBACK
    QWidget *userFeedBackWidget = new QWidget;
    userFeedBackWidget->setObjectName(QStringLiteral("userFeedBackWidget"));

    mUserFeedbackWidget = new KUserFeedback::FeedbackConfigWidget(this);

    auto *userFeedBackLayout = new QHBoxLayout(userFeedBackWidget);
    userFeedBackLayout->setContentsMargins({});
    userFeedBackLayout->addWidget(mUserFeedbackWidget);

    mUserFeedbackWidget->setFeedbackProvider(UserFeedBackManager::self()->userFeedbackProvider());
    KPageWidgetItem *userFeedBackPageWidgetPage = new KPageWidgetItem(userFeedBackWidget, i18n("User Feedback"));
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
    PimCommon::ConfigureImmutableWidgetUtils::loadWidget(mCloseWallet, SieveEditorGlobalConfig::self()->closeWalletItem());
    PimCommon::ConfigureImmutableWidgetUtils::loadWidget(mWrapText, SieveEditorGlobalConfig::self()->wrapTextItem());
}

void SieveEditorConfigureDialog::saveServerSieveConfig()
{
    mServerWidget->writeConfig();
    PimCommon::ConfigureImmutableWidgetUtils::saveCheckBox(mCloseWallet, SieveEditorGlobalConfig::self()->closeWalletItem());
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
    KConfigGroup group(KSharedConfig::openConfig(), "SieveEditorConfigureDialog");
    const QSize size = group.readEntry("Size", QSize(600, 400));
    if (size.isValid()) {
        resize(size);
    }
}

void SieveEditorConfigureDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openConfig(), "SieveEditorConfigureDialog");
    group.writeEntry("Size", size());
    group.sync();
}
