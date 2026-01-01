/*
   SPDX-FileCopyrightText: 2013-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "config-pim-sieve-editor.h"
#include <KPageDialog>
class QCheckBox;
class KConfigDialogManager;
class SieveEditorConfigureServerWidget;
#if WITH_KUSERFEEDBACK
namespace KUserFeedback
{
class FeedbackConfigWidget;
}
#endif

class SieveEditorConfigureDialog : public KPageDialog
{
    Q_OBJECT
public:
    explicit SieveEditorConfigureDialog(QWidget *parent = nullptr);
    ~SieveEditorConfigureDialog() override;

    void saveServerSieveConfig();

private:
    void readConfig();
    void writeConfig();
    void loadServerSieveConfig();
    SieveEditorConfigureServerWidget *mServerWidget = nullptr;
    QCheckBox *const mWrapText;
    KConfigDialogManager *m_configDialogManager = nullptr;
#if WITH_KUSERFEEDBACK
    KUserFeedback::FeedbackConfigWidget *mUserFeedbackWidget = nullptr;
#endif
};
