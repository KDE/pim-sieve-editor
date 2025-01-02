/*
   SPDX-FileCopyrightText: 2015-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QLabel>
#include <QList>

class SieveEditorEmptyTabWidgetLabel : public QLabel
{
    Q_OBJECT
public:
    explicit SieveEditorEmptyTabWidgetLabel(QWidget *parent = nullptr);
    ~SieveEditorEmptyTabWidgetLabel() override;

private:
    struct placeHolderTextInfo {
        placeHolderTextInfo() = default;

        placeHolderTextInfo(const QString &t, const QString &info)
            : text(t)
            , menuInfo(info)
        {
        }

        QString text;
        QString menuInfo;
    };

    void addInfo(QString &placeholderText, const QList<placeHolderTextInfo> &map);
    void init();
};
