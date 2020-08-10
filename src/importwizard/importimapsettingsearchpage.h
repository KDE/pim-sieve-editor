/*
   SPDX-FileCopyrightText: 2016-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef IMPORTIMAPSETTINGSEARCHPAGE_H
#define IMPORTIMAPSETTINGSEARCHPAGE_H

#include "libsieveeditor_private_export.h"
#include <QWidget>
class QListWidget;

class LIBSIEVEEDITOR_TESTS_EXPORT ImportImapSettingSearchPage : public QWidget
{
    Q_OBJECT
public:
    explicit ImportImapSettingSearchPage(QWidget *parent = nullptr);
    ~ImportImapSettingSearchPage();

    void setProgramList(const QStringList &lst);
    Q_REQUIRED_RESULT QStringList selectedPrograms() const;

Q_SIGNALS:
    void needToImportSettings(bool);

private:
    void slotItemChanged();
    QListWidget *mFoundProgramList = nullptr;
};

#endif // IMPORTIMAPSETTINGSEARCHPAGE_H
