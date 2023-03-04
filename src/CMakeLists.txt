# SPDX-FileCopyrightText: 2013-2023 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
configure_file(sieveeditor-version.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/sieveeditor-version.h @ONLY)

add_library(libsieveeditor)

kconfig_add_kcfg_files(libsieveeditor settings/sieveeditorglobalconfig.kcfgc)

target_sources(libsieveeditor PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/importwizard/importimapsettingwizard.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/importwizard/importimapsettingsearchpage.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/importwizard/importimapsettingnofoundpage.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/importwizard/importimapsettingprogresspage.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/importwizard/importimapsettingfinishpage.cpp
    )

target_sources(libsieveeditor PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/importwizard/checks/importimapsettingsakonadicheckjob.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/importwizard/checks/importimapsettingsthunderbirdcheckjob.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/importwizard/checks/abstractimapsettingscheckjob.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/importwizard/checks/abstractimapsettingspassword.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/importwizard/checks/importimapsettingsakonadipassword.cpp
    )

if (TARGET KUserFeedbackWidgets${KUSERFEEDBACK_SUFFIX})
    add_definitions(-DWITH_KUSERFEEDBACK)
endif()


if (TARGET KUserFeedbackWidgets${KUSERFEEDBACK_SUFFIX})
    target_sources(libsieveeditor PRIVATE
        userfeedback/userfeedbackmanager.cpp
        userfeedback/userfeedbackmanager.h
        )
endif()


target_sources(libsieveeditor PRIVATE
    sieveeditormainwindow.cpp
    serversievesettingsdialog.cpp
    serversievesettings.cpp
    sieveeditormainwidget.cpp
    sieveeditorconfiguredialog.cpp
    sieveeditorscriptmanagerwidget.cpp
    sieveeditorutil.cpp
    serversievelistwidget.cpp
    sieveeditorconfigureserverwidget.cpp
    sieveeditormanagesievewidget.cpp
    sieveeditorpagewidget.cpp
    sieveserversettings.cpp
    sieveeditortabwidget.cpp
    sieveeditorcentralwidget.cpp
    sieveeditorconfigureserverpage.cpp
    sieveeditorbookmarks.cpp
    sieveeditoremptytabwidgetlabel.cpp
    readserversieveconfigjob.cpp
    sieveeditorsavepasswordjob.cpp
    sieveeditor.qrc
    libsieveeditor_private_export.h
    sieveeditortabwidget.h
    sieveeditoremptytabwidgetlabel.h
    sieveeditormainwindow.h
    sieveserversettings.h
    sieveeditormainwidget.h
    sieveeditorconfiguredialog.h
    readserversieveconfigjob.h
    sieveeditorconfigureserverwidget.h
    serversievesettings.h
    serversievelistwidget.h
    sieveeditorscriptmanagerwidget.h
    sieveeditorcentralwidget.h
    sieveeditorutil.h
    serversievesettingsdialog.h
    sieveeditormanagesievewidget.h
    importwizard/importimapsettingfinishpage.h
    importwizard/checks/abstractimapsettingscheckjob.h
    importwizard/checks/importimapsettingsakonadicheckjob.h
    importwizard/checks/importimapsettingsthunderbirdcheckjob.h
    importwizard/checks/importimapsettingsakonadipassword.h
    importwizard/checks/abstractimapsettingspassword.h
    importwizard/importimapsettingwizard.h
    importwizard/importimapsettingprogresspage.h
    importwizard/importimapsettingnofoundpage.h
    importwizard/importimapsettingsearchpage.h
    sieveeditorsavepasswordjob.h
    sieveeditorbookmarks.h
    sieveeditorpagewidget.h
    sieveeditorconfigureserverpage.h
    sievepurposemenuwidget.cpp
    sievepurposemenuwidget.h
)

ki18n_wrap_ui(libsieveeditor
    ui/serversievesettings.ui
    ui/sieveeditorconfigureserverwidget.ui
    )
ecm_qt_declare_logging_category(libsieveeditor HEADER sieveeditor_debug.h
	IDENTIFIER SIEVEEDITOR_LOG 
	CATEGORY_NAME org.kde.pim.sieveeditor 
	OLD_CATEGORY_NAMES log_sieveeditor 
	DESCRIPTION "sieveeditor (sieveeditor)" EXPORT SIEVEEDITOR)

if (COMPILE_WITH_UNITY_CMAKE_SUPPORT)
    set_target_properties(libsieveeditor PROPERTIES UNITY_BUILD ON)
endif()
generate_export_header(libsieveeditor BASE_NAME libsieveeditor)

set(libsieveeditor_userfeedback_LIB)
if (TARGET KUserFeedbackWidgets${KUSERFEEDBACK_SUFFIX})
    set(libsieveeditor_userfeedback_LIB KUserFeedbackWidgets${KUSERFEEDBACK_SUFFIX})
endif()


target_link_libraries(libsieveeditor
    PRIVATE
    KPim${KF_MAJOR_VERSION}::MailTransport
    KF${KF_MAJOR_VERSION}::Bookmarks
    KF${KF_MAJOR_VERSION}::I18n
    KF${KF_MAJOR_VERSION}::XmlGui
    KF5::PimCommon
    KPim${KF_MAJOR_VERSION}::KSieveUi
    KPim${KF_MAJOR_VERSION}::KManageSieve
    ${libsieveeditor_userfeedback_LIB}
    )
target_link_libraries(libsieveeditor PRIVATE qt${QT_MAJOR_VERSION}keychain)

set_target_properties(libsieveeditor
    PROPERTIES OUTPUT_NAME sieveeditor VERSION ${KDEPIM_LIB_VERSION} SOVERSION ${KDEPIM_LIB_SOVERSION}
    )

set(sieveeditor_SRCS main.cpp)

add_executable(sieveeditor ${sieveeditor_SRCS} )

target_link_libraries(sieveeditor
    KF${KF_MAJOR_VERSION}::CoreAddons
    KF${KF_MAJOR_VERSION}::Crash
    libsieveeditor
    KF${KF_MAJOR_VERSION}::XmlGui
    KF${KF_MAJOR_VERSION}::I18n
    KF${KF_MAJOR_VERSION}::DBusAddons
    ${libsieveeditor_userfeedback_LIB}
    )

install(TARGETS sieveeditor ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})

install(PROGRAMS org.kde.sieveeditor.desktop DESTINATION ${KDE_INSTALL_APPDIR})


install(FILES settings/sieveeditorglobalconfig.kcfg DESTINATION ${KDE_INSTALL_KCFGDIR})
install(TARGETS libsieveeditor ${KDE_INSTALL_TARGETS_DEFAULT_ARGS} LIBRARY NAMELINK_SKIP)


if (BUILD_TESTING)
    add_subdirectory(importwizard/autotests)
    add_subdirectory(importwizard/tests)
endif()

install(FILES org.kde.sieveeditor.appdata.xml DESTINATION ${KDE_INSTALL_METAINFODIR})
ecm_qt_install_logging_categories(EXPORT SIEVEEDITOR FILE sieveeditor.categories DESTINATION ${KDE_INSTALL_LOGGINGCATEGORIESDIR})

