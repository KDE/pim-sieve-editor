/*
   SPDX-FileCopyrightText: 2014-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef SIEVEEDITORCENTRALWIDGET_H
#define SIEVEEDITORCENTRALWIDGET_H

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

#endif // SIEVEEDITORCENTRALWIDGET_H
