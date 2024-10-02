# SPDX-FileCopyrightText: 2013-2024 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
set(PIM_VERSION "6.2.40")
project( sieveeditor VERSION ${PIM_VERSION})
set(RELEASE_SERVICE_VERSION "24.11.40")


set(KF_MIN_VERSION "6.6.0")
set(QT_REQUIRED_VERSION "6.7.0")
set(QT6KEYCHAIN_LIB_VERSION "0.14.2")

find_package(ECM ${KF_MIN_VERSION} REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})
include(ECMInstallIcons)


include(ECMSetupVersion)
include(ECMAddTests)

include(GenerateExportHeader)
include(ECMGenerateHeaders)

include(FeatureSummary)
include(KDEGitCommitHooks)
include(KDEClangFormat)
file(GLOB_RECURSE ALL_CLANG_FORMAT_SOURCE_FILES *.cpp *.h *.c)
kde_clang_format(${ALL_CLANG_FORMAT_SOURCE_FILES})
include(CheckFunctionExists)

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(ECMAddAppIcon)
include(ECMQtDeclareLoggingCategory)
include(ECMDeprecationSettings)
include(ECMFeatureSummary)
include(ECMCheckOutboundLicense)
file(GLOB_RECURSE ALL_SOURCE_FILES
src/*.cpp
src/*.h
autotests/*.cpp
autotests/*.h
)
ecm_check_outbound_license(LICENSES GPL-2.0-only  FILES ${ALL_SOURCE_FILES})

# Do NOT add quote
set(KDEPIM_DEV_VERSION alpha)

# add an extra space
if(DEFINED KDEPIM_DEV_VERSION)
    set(KDEPIM_DEV_VERSION " ${KDEPIM_DEV_VERSION}")
endif()

set(KDEPIM_VERSION "${PIM_VERSION}${KDEPIM_DEV_VERSION} (${RELEASE_SERVICE_VERSION})")

set(KMAILTRANSPORT_LIB_VERSION "6.2.40")

set(KDEPIM_LIB_VERSION "${PIM_VERSION}")
set(KDEPIM_LIB_SOVERSION "6")

set(LIBKSIEVE_LIB_VERSION "6.2.40")
set(PIMCOMMON_LIB_VERSION "6.2.40")
set(KIMAP_LIB_VERSION "6.2.40")


find_package(Qt6 ${QT_REQUIRED_VERSION} CONFIG REQUIRED Widgets Network)

# Find KF6 package
find_package(KF6CoreAddons ${KF_MIN_VERSION} REQUIRED)
find_package(KF6Crash ${KF_MIN_VERSION} REQUIRED)
find_package(KF6Bookmarks ${KF_MIN_VERSION} REQUIRED)
find_package(KF6KIO ${KF_MIN_VERSION} REQUIRED)
find_package(KF6XmlGui ${KF_MIN_VERSION} REQUIRED)
find_package(KF6DocTools ${KF_MIN_VERSION})
find_package(KF6I18n ${KF_MIN_VERSION})
find_package(KF6IconThemes ${KF_MIN_VERSION} QUIET)
set_package_properties(KF6DocTools PROPERTIES DESCRIPTION
    "Tools to generate documentation"
    TYPE OPTIONAL
)
find_package(KF6UserFeedback ${KF_MIN_VERSION} CONFIG)
set_package_properties(KF6::UserFeedback PROPERTIES DESCRIPTION "User Feedback lib" TYPE OPTIONAL PURPOSE "Allow to send Telemetry Information (optional).")

if(NOT WIN32 AND NOT APPLE) 
   find_package(KF6DBusAddons ${KF_MIN_VERSION} CONFIG REQUIRED)
endif()



# Find KdepimLibs Package
find_package(KPim6MailTransport ${KMAILTRANSPORT_LIB_VERSION} CONFIG REQUIRED)

find_package(KPim6PimCommon ${PIMCOMMON_LIB_VERSION} CONFIG REQUIRED)
find_package(KPim6KSieveUi ${LIBKSIEVE_LIB_VERSION} CONFIG REQUIRED)
find_package(KPim6IMAP ${KIMAP_LIB_VERSION} CONFIG REQUIRED)

find_package(Qt6Keychain ${QT6KEYCHAIN_LIB_VERSION} CONFIG)
set_package_properties(Qt6Keychain PROPERTIES
                                   DESCRIPTION "Provides support for secure credentials storage"
                                   URL "https://github.com/frankosterfeld/qtkeychain"
                                   TYPE REQUIRED)



include_directories(${sieveeditor_SOURCE_DIR} ${sieveeditor_BINARY_DIR})

add_definitions(-DQT_NO_CONTEXTLESS_CONNECT)
ecm_set_disabled_deprecation_versions(QT 6.7.2  KF 6.7.0)


option(USE_UNITY_CMAKE_SUPPORT "Use UNITY cmake support (speedup compile time)" OFF)

set(COMPILE_WITH_UNITY_CMAKE_SUPPORT OFF)
if(USE_UNITY_CMAKE_SUPPORT)
    set(COMPILE_WITH_UNITY_CMAKE_SUPPORT ON)
endif()

if(BUILD_TESTING)
   add_definitions(-DBUILD_TESTING)
   find_package(Qt6 ${QT_REQUIRED_VERSION} CONFIG REQUIRED Test)
endif()
add_subdirectory(src)
if(BUILD_TESTING)
    add_subdirectory(autotests)
endif()

kde_configure_git_pre_commit_hook(CHECKS CLANG_FORMAT)
ki18n_install(po)

if(KF6DocTools_FOUND)
    kdoctools_install(po)
    add_subdirectory(doc)
endif()

ecm_feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)

