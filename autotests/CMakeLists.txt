# SPDX-FileCopyrightText: 2013-2025 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
add_executable(serversievesettingstest serversievesettingstest.cpp)
add_test(NAME serversievesettingstest COMMAND serversievesettingstest)
ecm_mark_as_test(serversievesettingstest)
target_link_libraries(
    serversievesettingstest
    Qt::Test
    Qt::Widgets
    KPim6::MailTransport
    KF6::I18n
    KF6::ConfigGui
    KPim6::KSieveUi
    libsieveeditor
)

set(sieveeditorutiltest_source sieveeditorutiltest.cpp)
add_executable(sieveeditorutiltest ${sieveeditorutiltest_source})
add_test(NAME sieveeditorutiltest COMMAND sieveeditorutiltest)
ecm_mark_as_test(sieveeditorutiltest)
target_link_libraries(
    sieveeditorutiltest
    Qt::Test
    KPim6::MailTransport
    KF6::I18n
    KF6::ConfigGui
    KPim6::KSieveUi
    libsieveeditor
)

# convenience macro to add qtest unit tests
macro(add_sieveserver_unittest _source)
    set(_test ${_source})
    get_filename_component(_name ${_source} NAME_WE)
    add_executable(${_name} ${_test})
    add_test(NAME ${_name} COMMAND ${_name})
    ecm_mark_as_test(sieveserver-${_name})
    target_link_libraries(
        ${_name}
        Qt::Test
        Qt::Widgets
        KF6::I18n
        KPim6::MailTransport
        KPim6::KSieveUi
        libsieveeditor
    )
endmacro()

add_sieveserver_unittest( sieveeditorconfigureserverpagetest.cpp)
