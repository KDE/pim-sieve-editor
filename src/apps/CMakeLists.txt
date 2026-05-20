# SPDX-FileCopyrightText: 2013-2026 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause

add_executable(sieveeditor)

# application icon, in all sizes needed to have e.g. nice Windows icons, too
set(ICON_FILES
    icons/sc-apps-sieveeditor.svg
    icons/16-apps-sieveeditor.png
    icons/22-apps-sieveeditor.png
    icons/32-apps-sieveeditor.png
    icons/48-apps-sieveeditor.png
    icons/64-apps-sieveeditor.png
)

# Add icon files to the application's source files to have CMake bundle them in the executable.
set(ICONS_SOURCES)
ecm_add_app_icon(ICONS_SOURCES ICONS ${ICON_FILES})
target_sources(
    sieveeditor
    PRIVATE
        main.cpp
        ${ICONS_SOURCES}
)

target_link_libraries(
    sieveeditor
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
    if(NOT WIN32 AND NOT APPLE)
        target_link_libraries(sieveeditor Qt::GuiPrivate)
    endif()
endif()
install(
    TARGETS
        sieveeditor
        ${KDE_INSTALL_TARGETS_DEFAULT_ARGS}
)

add_subdirectory(icons)
