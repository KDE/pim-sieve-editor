configure_file(sieveeditor-version.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/sieveeditor-version.h @ONLY)


set( sieveeditor_kcfg_SRCS settings/sieveeditorglobalconfig.kcfgc )

kconfig_add_kcfg_files(sieveeditor_kcfg_SRCS ${sieveeditor_kcfg_SRCS})

set(sieveeditor_importimapsettings_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/importwizard/importimapsettingwizard.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/importwizard/importimapsettingsearchpage.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/importwizard/importimapsettingnofoundpage.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/importwizard/importimapsettingprogresspage.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/importwizard/importimapsettingfinishpage.cpp
    )

set(sieveeditor_importimapsettings_check_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/importwizard/checks/importimapsettingsakonadicheckjob.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/importwizard/checks/importimapsettingsthunderbirdcheckjob.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/importwizard/checks/abstractimapsettingscheckjob.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/importwizard/checks/abstractimapsettingspassword.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/importwizard/checks/importimapsettingsakonadipassword.cpp
    )

set(sieveeditor_userfeedback_SRCS)
if (TARGET KUserFeedbackWidgets)
    set(sieveeditor_userfeedback_SRCS ${sieveeditor_userfeedback_SRCS}
        #TODO
        )
endif()


set(sieveeditor_LIB_SRCS
    ${sieveeditor_userfeedback_SRCS}
    ${sieveeditor_kcfg_SRCS}
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
    sievepurposemenuwidget.cpp
    ${sieveeditor_importimapsettings_SRCS}
    ${sieveeditor_importimapsettings_check_SRCS}
    )


ki18n_wrap_ui(sieveeditor_LIB_SRCS
    ui/serversievesettings.ui
    ui/sieveeditorconfigureserverwidget.ui
    )

qt5_add_resources(sieveeditor_LIB_SRCS sieveeditor.qrc)

ecm_qt_declare_logging_category(sieveeditor_LIB_SRCS HEADER sieveeditor_debug.h IDENTIFIER SIEVEEDITOR_LOG CATEGORY_NAME org.kde.pim.sieveeditor)

add_library(libsieveeditor ${sieveeditor_LIB_SRCS})

generate_export_header(libsieveeditor BASE_NAME libsieveeditor)

target_link_libraries(libsieveeditor
    PRIVATE
    KF5::MailTransport
    KF5::Bookmarks
    KF5::I18n
    KF5::IconThemes
    KF5::XmlGui
    KF5::PimCommon
    KF5::KSieveUi
    KF5::KManageSieve
    )

set_target_properties(libsieveeditor
    PROPERTIES OUTPUT_NAME sieveeditor VERSION ${KDEPIM_LIB_VERSION} SOVERSION ${KDEPIM_LIB_SOVERSION}
    )

set(sieveeditor_SRCS main.cpp)

add_executable(sieveeditor ${sieveeditor_SRCS} )

target_link_libraries(sieveeditor
    KF5::Crash
    libsieveeditor
    KF5::XmlGui
    KF5::I18n
    KF5::DBusAddons
    )

install(TARGETS sieveeditor ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})

install(PROGRAMS org.kde.sieveeditor.desktop DESTINATION ${KDE_INSTALL_APPDIR})

add_subdirectory(kconf_update)

install(FILES settings/sieveeditorglobalconfig.kcfg DESTINATION ${KDE_INSTALL_KCFGDIR})
install(TARGETS libsieveeditor ${KDE_INSTALL_TARGETS_DEFAULT_ARGS} LIBRARY NAMELINK_SKIP)


if (BUILD_TESTING)
    add_subdirectory(importwizard/autotests)
    add_subdirectory(importwizard/tests)
endif()

install(FILES org.kde.sieveeditor.appdata.xml DESTINATION ${KDE_INSTALL_METAINFODIR})

