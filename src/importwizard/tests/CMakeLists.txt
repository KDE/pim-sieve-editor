# SPDX-FileCopyrightText: 2013-2023 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
add_executable(sieveeditorimportwizardtest main.cpp)

target_link_libraries(sieveeditorimportwizardtest
    Qt::Widgets
    KF${KF_MAJOR_VERSION}::WidgetsAddons
    KF${KF_MAJOR_VERSION}::I18n
    KF${KF_MAJOR_VERSION}::ConfigCore
    KF${KF_MAJOR_VERSION}::ConfigGui
    KPim${KF_MAJOR_VERSION}::MailTransport
    KF5::KSieveUi
    libsieveeditor
    )

