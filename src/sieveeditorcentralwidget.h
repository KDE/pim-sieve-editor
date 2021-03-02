/*
   SPDX-FileCopyrightText: 2014-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QStackedWidget>
class SieveEditorConfigureServerPage;
class SieveEditorMainWidget;
class KActionCollection;
class SieveEditorCentralWidget : public QStackedWidget
{
    Q_OBJECT
public:
    explicit SieveEditorCentralWidget(QWidget *parent = nullptr, KActionCollection *ac = nullptr);
    ~SieveEditorCentralWidget();

    SieveEditorMainWidget *sieveEditorMainWidget() const;

Q_SIGNALS:
    void configureClicked();
    void importSieveSettings();

private:
    void slotServerSieveFound(bool hasServer);
    SieveEditorConfigureServerPage *mConfigureWidget = nullptr;
    SieveEditorMainWidget *mSieveEditorMainWidget = nullptr;
};

