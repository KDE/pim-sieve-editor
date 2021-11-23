/*
   SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "sieveeditoremptytabwidgetlabel.h"
#include <KLocalizedString>

SieveEditorEmptyTabWidgetLabel::SieveEditorEmptyTabWidgetLabel(QWidget *parent)
    : QLabel(parent)
{
    init();
}

SieveEditorEmptyTabWidgetLabel::~SieveEditorEmptyTabWidgetLabel() = default;

void SieveEditorEmptyTabWidgetLabel::init()
{
    // TODO improve text
    QString placeholderText = QStringLiteral(
        "<html><body style=\"color:#909090; font-size:14px\">"
        "<div align='center'>");
    const QVector<placeHolderTextInfo> map{
        placeHolderTextInfo(i18n("Import script:"), i18nc("Action is from file menu, import submenu", "File > Import")),
        placeHolderTextInfo(i18n("Create Rules Graphically:"), i18nc("Action is from menu tools, submenu autogenerate script", "Tools > Autogenerate script")),
        placeHolderTextInfo(i18n("Debug a script:"), i18nc("These action in from menu tools submenu debug sieve script", "Tools > Debug Sieve Script")),
        placeHolderTextInfo(i18n("Import Sieve Settings:"), i18nc("Action is from tools menu, import IMAP settings", "Tools > Import IMAP settings"))};
    addInfo(placeholderText, map);
    placeholderText += QStringLiteral(
        "<div></div>"
        "</div>"
        "</body></html>");
    setTextFormat(Qt::RichText);
    setText(placeholderText);
}

void SieveEditorEmptyTabWidgetLabel::addInfo(QString &placeholderText, const QVector<placeHolderTextInfo> &map)
{
    for (int i = 0; i < map.size(); ++i) {
        placeholderText += QStringLiteral(
                               "<div style=\"font-size:20px\">%1</div>"
                               "<div></div>"
                               "<li>%2")
                               .arg(map.at(i).text, map.at(i).menuInfo);
    }
}
