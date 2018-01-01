/*
   Copyright (C) 2015-2018 Montel Laurent <montel@kde.org>

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

#include "sieveeditoremptytabwidgetlabel.h"
#include <KLocalizedString>

SieveEditorEmptyTabWidgetLabel::SieveEditorEmptyTabWidgetLabel(QWidget *parent)
    : QLabel(parent)
{
    init();
}

SieveEditorEmptyTabWidgetLabel::~SieveEditorEmptyTabWidgetLabel()
{
}

void SieveEditorEmptyTabWidgetLabel::init()
{
    //TODO improve text
    QString placeholderText = QStringLiteral("<html><body style=\"color:#909090; font-size:14px\">"
                                             "<div align='center'>");
    const QVector<placeHolderTextInfo> map {
        placeHolderTextInfo(i18n("Import script:"), i18nc("Action is from file menu, import submenu", "File > Import")),
        placeHolderTextInfo(i18n("Create Rules Graphically:"), i18nc("Action is from menu tools, submenu autogenerate script", "Tools > Autogenerate script")),
        placeHolderTextInfo(i18n("Debug a script:"), i18nc("These action in from menu tools submenu debug sieve script", "Tools > Debug Sieve Script")),
        placeHolderTextInfo(i18n("Import Sieve Settings"), i18nc("Action is from tools menu, import IMAP settings", "Tools > Import IMAP settings"))
    };
    addInfo(placeholderText, map);
    placeholderText += QStringLiteral("<div></div>"
                                      "</div>"
                                      "</body></html>");
    setTextFormat(Qt::RichText);
    setText(placeholderText);
}

void SieveEditorEmptyTabWidgetLabel::addInfo(QString &placeholderText, const QVector<placeHolderTextInfo> &map)
{
    for (int i = 0; i < map.size(); ++i) {
        placeholderText += QStringLiteral("<div style=\"font-size:20px\">%1</div>"
                                          "<div></div>"
                                          "<li>%2").arg(map.at(i).text, map.at(i).menuInfo);
    }
}
