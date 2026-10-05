/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "sieveeditorconfigurespeechtotextwidget.h"
#include "sieveeditorglobalconfig.h"

#include <TextSpeechToText/SpeechToTextConfigureWidget>

#include <KLocalizedString>

#include <QCheckBox>
#include <QVBoxLayout>

using namespace Qt::Literals::StringLiterals;
SieveEditorConfigureSpeechToTextWidget::SieveEditorConfigureSpeechToTextWidget(QWidget *parent)
    : QWidget{parent}
    , mSpeechToTextWidget(new TextSpeechToText::SpeechToTextConfigureWidget(this))
    , mEnableSpeechToText(new QCheckBox(i18nc("@option:check", "Enable Speech To Text"), this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    mEnableSpeechToText->setObjectName(u"mEnableSpeechToText"_s);
    mainLayout->addWidget(mEnableSpeechToText);

    mSpeechToTextWidget->setObjectName(u"mSpeechToTextWidget"_s);
    mainLayout->addWidget(mSpeechToTextWidget);
    mainLayout->addStretch(1);

    connect(mEnableSpeechToText, &QCheckBox::toggled, mSpeechToTextWidget, &TextSpeechToText::SpeechToTextConfigureWidget::setEnabled);
}

SieveEditorConfigureSpeechToTextWidget::~SieveEditorConfigureSpeechToTextWidget() = default;

void SieveEditorConfigureSpeechToTextWidget::save()
{
    SieveEditorGlobalConfig::self()->setEnableSpeechToText(mEnableSpeechToText->isChecked());
    mSpeechToTextWidget->saveSettings();
}

void SieveEditorConfigureSpeechToTextWidget::load()
{
    mSpeechToTextWidget->loadSettings();
    mEnableSpeechToText->setChecked(SieveEditorGlobalConfig::self()->enableSpeechToText());
    mSpeechToTextWidget->setEnabled(mEnableSpeechToText->isChecked());
}

#include "moc_sieveeditorconfigurespeechtotextwidget.cpp"
