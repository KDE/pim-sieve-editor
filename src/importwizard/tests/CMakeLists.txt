# SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
add_executable(sieveeditorimportwizardtest main.cpp)

target_link_libraries(sieveeditorimportwizardtest
    Qt::Widgets
    KF5::WidgetsAddons
    KF5::I18n
    KF5::ConfigCore
    KF5::ConfigGui
    KF5::MailTransport
    KF5::KSieveUi
    libsieveeditor
    )

