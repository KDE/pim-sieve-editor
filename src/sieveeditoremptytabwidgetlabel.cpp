/*
   SPDX-FileCopyrightText: 2015-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "sieveeditoremptytabwidgetlabel.h"
#include <KLocalizedString>
#include <QHBoxLayout>
#include <QLabel>

SieveEditorEmptyTabWidgetLabel::SieveEditorEmptyTabWidgetLabel(QWidget *parent)
    : QWidget(parent)
{
    auto mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins({});

    auto label = new QLabel(this);

    QString placeholderText = QStringLiteral(
        "<html><body style=\"color:#909090; font-size:14px\">"
        "<div align='left'>");
    const QList<placeHolderTextInfo> map{
        placeHolderTextInfo(i18n("Import script:"), i18nc("Action is from file menu, import submenu", "File > Import")),
        placeHolderTextInfo(i18n("Create Rules Graphically:"), i18nc("Action is from menu tools, submenu autogenerate script", "Tools > Autogenerate script")),
        placeHolderTextInfo(i18n("Debug a script:"), i18nc("These action in from menu tools submenu debug sieve script", "Tools > Debug Sieve Script")),
        placeHolderTextInfo(i18n("Import Sieve Settings:"), i18nc("Action is from tools menu, import IMAP settings", "Tools > Import IMAP settings"))};
    addInfo(placeholderText, map);
    placeholderText += QStringLiteral(
        "<div></div>"
        "</div>"
        "</body></html>");
    label->setTextFormat(Qt::RichText);
    label->setText(placeholderText);
    mainLayout->addWidget(label, 0, Qt::AlignHCenter);
}

SieveEditorEmptyTabWidgetLabel::~SieveEditorEmptyTabWidgetLabel() = default;

void SieveEditorEmptyTabWidgetLabel::addInfo(QString &placeholderText, const QList<placeHolderTextInfo> &map)
{
    placeholderText += QStringLiteral("<ul>");
    for (const placeHolderTextInfo &info : map) {
        placeholderText += QStringLiteral(
                               "<li><div style=\"font-size:20px\">%1</div>"
                               "<div>%2</div></li>")
                               .arg(info.text.toHtmlEscaped(), info.menuInfo.toHtmlEscaped());
    }
    placeholderText += QStringLiteral("</ul>");
}

#include "moc_sieveeditoremptytabwidgetlabel.cpp"
