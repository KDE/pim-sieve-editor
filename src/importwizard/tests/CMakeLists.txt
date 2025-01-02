# SPDX-FileCopyrightText: 2013-2025 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
add_executable(sieveeditorimportwizardtest main.cpp)

target_link_libraries(sieveeditorimportwizardtest
    Qt::Widgets
    KF6::WidgetsAddons
    KF6::I18n
    KF6::ConfigCore
    KF6::ConfigGui
    KPim6::MailTransport
    KPim6::KSieveUi
    libsieveeditor
)

