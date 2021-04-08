# SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>
# SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
set(sieveeditorimportwizardtest_SRCS main.cpp)

add_executable(sieveeditorimportwizardtest ${sieveeditorimportwizardtest_SRCS})

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

