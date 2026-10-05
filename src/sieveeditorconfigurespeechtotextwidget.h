/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "libsieveeditor_private_export.h"
#include <QWidget>
namespace TextSpeechToText
{
class SpeechToTextConfigureWidget;
}
class QCheckBox;
class LIBSIEVEEDITOR_TESTS_EXPORT SieveEditorConfigureSpeechToTextWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SieveEditorConfigureSpeechToTextWidget(QWidget *parent = nullptr);
    ~SieveEditorConfigureSpeechToTextWidget() override;

    void save();
    void load();

private:
    TextSpeechToText::SpeechToTextConfigureWidget *const mSpeechToTextWidget;
    QCheckBox *const mEnableSpeechToText;
};
