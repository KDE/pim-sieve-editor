/*
   SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <KPageDialog>
class QCheckBox;
class SieveEditorConfigureServerWidget;
#ifdef WITH_KUSERFEEDBACK
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
    QCheckBox *mWrapText = nullptr;
#ifdef WITH_KUSERFEEDBACK
    KUserFeedback::FeedbackConfigWidget *mUserFeedbackWidget = nullptr;
#endif
};

