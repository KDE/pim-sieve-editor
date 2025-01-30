# SPDX-FileCopyrightText: 2013-2025 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause

if(TARGET KF6::UserFeedbackWidgets)
    set(WITH_KUSERFEEDBACK true)
endif()


configure_file(sieveeditor-version.h.in ${CMAKE_CURRENT_BINARY_DIR}/sieveeditor-version.h @ONLY)

add_library(libsieveeditor)

configure_file(config-pim-sieve-editor.h.in ${CMAKE_CURRENT_BINARY_DIR}/config-pim-sieve-editor.h)

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

if(TARGET KF6::UserFeedbackWidgets)
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

if(COMPILE_WITH_UNITY_CMAKE_SUPPORT)
    set_target_properties(libsieveeditor PROPERTIES UNITY_BUILD ON)
endif()
generate_export_header(libsieveeditor BASE_NAME libsieveeditor)

set(libsieveeditor_userfeedback_LIB)
if(TARGET KF6::UserFeedbackWidgets)
    set(libsieveeditor_userfeedback_LIB KF6::UserFeedbackWidgets)
endif()


target_link_libraries(libsieveeditor
    PRIVATE
    KPim6::MailTransport
    KF6::Bookmarks
    KF6::I18n
    KF6::XmlGui
    KF6::BookmarksWidgets
    KPim6::PimCommon
    KPim6::KSieveUi
    KPim6::KManageSieve
    KF6::WindowSystem
    ${libsieveeditor_userfeedback_LIB}
)
target_link_libraries(libsieveeditor PRIVATE qt6keychain)

set_target_properties(libsieveeditor
    PROPERTIES OUTPUT_NAME sieveeditor VERSION ${KDEPIM_LIB_VERSION} SOVERSION ${KDEPIM_LIB_SOVERSION}
)

add_executable(sieveeditor)

# application icon, in all sizes needed to have e.g. nice Windows icons, too
set(ICON_FILES
  icons/sc-apps-sieveeditor.svg
  icons/16-apps-sieveeditor.png
  icons/22-apps-sieveeditor.png
  icons/32-apps-sieveeditor.png
  icons/44-apps-sieveeditor.png
  icons/48-apps-sieveeditor.png
  icons/64-apps-sieveeditor.png
  icons/150-apps-sieveeditor.png
)

# Add icon files to the application's source files to have CMake bundle them in the executable.
ecm_add_app_icon(ICONS_SOURCES ICONS ${ICON_FILES})
target_sources(sieveeditor PRIVATE main.cpp ${ICONS_SOURCES})

target_link_libraries(sieveeditor
    KF6::CoreAddons
    KF6::Crash
    libsieveeditor
    KF6::XmlGui
    KF6::I18n
    ${libsieveeditor_userfeedback_LIB}
    KF6::IconThemes
    KF6::WindowSystem
)

if(TARGET KF6::DBusAddons)
    target_link_libraries(sieveeditor KF6::DBusAddons)
else()
    target_link_libraries(sieveeditor KDAB::kdsingleapplication)
    if (NOT WIN32 AND NOT APPLE)
        target_link_libraries(sieveeditor Qt::GuiPrivate)
    endif()
endif()    	
install(TARGETS sieveeditor ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})

install(PROGRAMS org.kde.sieveeditor.desktop DESTINATION ${KDE_INSTALL_APPDIR})


install(FILES settings/sieveeditorglobalconfig.kcfg DESTINATION ${KDE_INSTALL_KCFGDIR})
install(TARGETS libsieveeditor ${KDE_INSTALL_TARGETS_DEFAULT_ARGS} LIBRARY NAMELINK_SKIP)


if(BUILD_TESTING)
    add_subdirectory(importwizard/autotests)
    add_subdirectory(importwizard/tests)
endif()

install(FILES org.kde.sieveeditor.appdata.xml DESTINATION ${KDE_INSTALL_METAINFODIR})
ecm_qt_install_logging_categories(EXPORT SIEVEEDITOR FILE sieveeditor.categories DESTINATION ${KDE_INSTALL_LOGGINGCATEGORIESDIR})

add_subdirectory(icons)

# See https://cmake.org/cmake/help/v3.15/prop_tgt/MACOSX_BUNDLE_INFO_PLIST.html
if(APPLE)
  set_property(
    TARGET sieveeditor
    PROPERTY MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/data/MacOSXBundleInfo.plist.in
  )

  # These are substituted by CMake into plist.in.
  set(MACOSX_BUNDLE_DISPLAY_NAME "SieveEditor")
  set(MACOSX_BUNDLE_GUI_IDENTIFIER "org.kde.sieveeditor")
  set(MACOSX_BUNDLE_BUNDLE_NAME "SieveEditor")
  set(MACOSX_BUNDLE_DISPLAY_NAME "SieveEditor")
  set(MACOSX_BUNDLE_INFO_STRING "SieveEditor - Sieve Editor")
  set(MACOSX_BUNDLE_LONG_VERSION_STRING "SieveEditor ${PIM_VERSION}")
  set(MACOSX_BUNDLE_SHORT_VERSION_STRING "${PIM_VERSION}")
  set(MACOSX_BUNDLE_BUNDLE_VERSION "${PIM_VERSION}")
  set(MACOSX_BUNDLE_COPYRIGHT "2013-2024 The SieveEditor Authors")
endif()

