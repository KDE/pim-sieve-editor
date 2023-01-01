/*
   SPDX-FileCopyrightText: 2016-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "libsieveeditor_private_export.h"
#include <QWidget>
class QListWidget;

class LIBSIEVEEDITOR_TESTS_EXPORT ImportImapSettingSearchPage : public QWidget
{
    Q_OBJECT
public:
    explicit ImportImapSettingSearchPage(QWidget *parent = nullptr);
    ~ImportImapSettingSearchPage() override;

    void setProgramList(const QStringList &lst);
    Q_REQUIRED_RESULT QStringList selectedPrograms() const;

Q_SIGNALS:
    void needToImportSettings(bool);

private:
    void slotItemChanged();
    QListWidget *mFoundProgramList = nullptr;
};
