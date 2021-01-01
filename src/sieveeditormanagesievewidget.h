/*
   Copyright (C) 2013-2021 Laurent Montel <montel@kde.org>

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

#ifndef SIEVEEDITORMANAGESIEVEWIDGET_H
#define SIEVEEDITORMANAGESIEVEWIDGET_H

#include "sieveeditorutil.h"
#include <KSieveUi/ManageSieveWidget>

class SieveEditorManageSieveWidget : public KSieveUi::ManageSieveWidget
{
    Q_OBJECT
public:
    explicit SieveEditorManageSieveWidget(QWidget *parent = nullptr);
    ~SieveEditorManageSieveWidget() override;

protected:
    Q_REQUIRED_RESULT bool refreshList() override;
    void updateSieveSettings() override;
private:
    void slotReadServerSieveConfigDone(const QVector<SieveEditorUtil::SieveServerConfig> &lstConfig);
    QVector<SieveEditorUtil::SieveServerConfig> mSieveServerSettings;
};

#endif // SIEVEEDITORMANAGESIEVEWIDGET_H
