/*
  Copyright (c) 2014 Montel Laurent <montel@kde.org>

  This library is free software; you can redistribute it and/or modify it
  under the terms of the GNU Library General Public License as published by
  the Free Software Foundation; either version 2 of the License, or (at your
  option) any later version.

  This library is distributed in the hope that it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public
  License for more details.

  You should have received a copy of the GNU Library General Public License
  along with this library; see the file COPYING.LIB.  If not, write to the
  Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
  02110-1301, USA.

*/

#ifndef SIEVEEDITORPAGEWIDGET_H
#define SIEVEEDITORPAGEWIDGET_H

#include "editor/sieveeditorwidget.h"
#include <KUrl>
namespace KManageSieve {
class SieveJob;
}

class SieveEditorPageWidget : public KSieveUi::SieveEditorWidget
{
    Q_OBJECT
public:
    explicit SieveEditorPageWidget(QWidget *parent=0);
    ~SieveEditorPageWidget();

    void loadScript(const KUrl &url, const QStringList &capabilities);

    KUrl currentUrl() const;

    void setIsNewScript(bool isNewScript);

    void addFailedMessage(const QString &err);
    void addOkMessage(const QString &err);
private slots:
    void slotGetResult(KManageSieve::SieveJob *, bool success, const QString &script, bool isActive);
    void slotCheckSyntaxClicked();
    void slotPutResultDebug(KManageSieve::SieveJob *, bool success, const QString &errorMsg);

private:
    void addMessageEntry(const QString &errorMsg, const QColor &color);
    KUrl mCurrentURL;
    bool mWasActive;
    bool mIsNewScript;
};

#endif // SIEVEEDITORPAGEWIDGET_H
