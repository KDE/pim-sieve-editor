/*
   SPDX-FileCopyrightText: 2014-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QStackedWidget>
class SieveEditorConfigureServerPage;
class SieveEditorMainWidget;
class KActionCollection;
namespace PimCommon
{
class PurposeMenuMessageWidget;
}
class SieveEditorCentralWidget : public QStackedWidget
{
    Q_OBJECT
public:
    explicit SieveEditorCentralWidget(QWidget *parent = nullptr, KActionCollection *ac = nullptr);
    ~SieveEditorCentralWidget() override;

    SieveEditorMainWidget *sieveEditorMainWidget() const;

    void slotShareError(const QString &message);
    void slotShareSuccess(const QString &message);
Q_SIGNALS:
    void configureClicked();
    void importSieveSettings();

private:
    void slotServerSieveFound(bool hasServer);
    SieveEditorConfigureServerPage *const mConfigureWidget;
    SieveEditorMainWidget *const mSieveEditorMainWidget;
    PimCommon::PurposeMenuMessageWidget *const mPurposeMenuMessageWidget;
    QWidget *const mMainWidget;
};
