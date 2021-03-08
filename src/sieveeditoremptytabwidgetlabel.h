/*
   SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QLabel>
#include <QVector>

class SieveEditorEmptyTabWidgetLabel : public QLabel
{
    Q_OBJECT
public:
    explicit SieveEditorEmptyTabWidgetLabel(QWidget *parent = nullptr);
    ~SieveEditorEmptyTabWidgetLabel() override;

private:
    struct placeHolderTextInfo {
        placeHolderTextInfo()
        {
        }

        placeHolderTextInfo(const QString &t, const QString &info)
            : text(t)
            , menuInfo(info)
        {
        }

        QString text;
        QString menuInfo;
    };

    void addInfo(QString &placeholderText, const QVector<placeHolderTextInfo> &map);
    void init();
};

