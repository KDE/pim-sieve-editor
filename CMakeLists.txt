project( sieveeditor )
cmake_minimum_required(VERSION 3.1)
if (POLICY CMP0063)
    cmake_policy(SET CMP0063 NEW)
endif()

set(KF5_VERSION "5.37.0")
find_package(ECM ${KF5_VERSION} REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})
include(ECMInstallIcons)

include(ECMOptionalAddSubdirectory)
include(ECMSetupVersion)
include(ECMAddTests)
include(ECMMarkNonGuiExecutable)
include(GenerateExportHeader)
include(ECMGenerateHeaders)
include(CMakePackageConfigHelpers)
include(FeatureSummary)
include(CheckFunctionExists)
include(ECMGeneratePriFile)

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(ECMAddAppIcon)
include(ECMQtDeclareLoggingCategory)
include(ECMCoverageOption)
# Do NOT add quote
set(KDEPIM_DEV_VERSION alpha1)

# add an extra space
if(DEFINED KDEPIM_DEV_VERSION)
    set(KDEPIM_DEV_VERSION " ${KDEPIM_DEV_VERSION}")
endif()


set(KDEPIM_VERSION_NUMBER "5.6.42")
set(KDEPIM_VERSION "${KDEPIM_VERSION_NUMBER}${KDEPIM_DEV_VERSION}")

set(KMAILTRANSPORT_LIB_VERSION "5.6.40")
set(KPIMTEXTEDIT_LIB_VERSION "5.6.40")

set(KDEPIM_LIB_VERSION "${KDEPIM_VERSION_NUMBER}")
set(KDEPIM_LIB_SOVERSION "5")

set(LIBKSIEVE_LIB_VERSION_LIB "5.6.42")
set(PIMCOMMON_LIB_VERSION_LIB "5.6.41")
set(KIMAP_LIB_VERSION "5.6.40")


set(QT_REQUIRED_VERSION "5.7.0")
find_package(Qt5 ${QT_REQUIRED_VERSION} CONFIG REQUIRED Widgets Network Test)

# Find KF5 package
find_package(KF5DBusAddons ${KF5_VERSION} CONFIG REQUIRED)
find_package(KF5DocTools ${KF5_VERSION} REQUIRED)
find_package(KF5Crash ${KF5_VERSION} REQUIRED)
find_package(KF5Bookmarks ${KF5_VERSION} REQUIRED)
find_package(KF5IconThemes ${KF5_VERSION} REQUIRED)
find_package(KF5KIO ${KF5_VERSION} REQUIRED)


# Find KdepimLibs Package
find_package(KF5PimTextEdit ${KPIMTEXTEDIT_LIB_VERSION} CONFIG REQUIRED)
find_package(KF5MailTransport ${KMAILTRANSPORT_LIB_VERSION} CONFIG REQUIRED)

find_package(KF5PimCommon ${PIMCOMMON_LIB_VERSION_LIB} CONFIG REQUIRED)
find_package(KF5LibKSieve ${LIBKSIEVE_LIB_VERSION_LIB} CONFIG REQUIRED)
find_package(KF5IMAP ${KIMAP_LIB_VERSION} CONFIG REQUIRED)

include_directories(${sieveeditor_SOURCE_DIR} ${sieveeditor_BINARY_DIR})

add_definitions(-DQT_NO_URL_CAST_FROM_STRING)
add_definitions(-DQT_USE_QSTRINGBUILDER)
add_definitions(-DQT_NO_CAST_FROM_ASCII)
add_definitions(-DQT_NO_CAST_TO_ASCII)
add_definitions(-DQT_NO_NARROWING_CONVERSIONS_IN_CONNECT)
add_definitions(-DQT_DISABLE_DEPRECATED_BEFORE=0x060000)
if(BUILD_TESTING)
   add_definitions(-DBUILD_TESTING)
endif(BUILD_TESTING)


add_subdirectory(src)
if (BUILD_TESTING)
    add_subdirectory(autotests)
endif()

install( FILES sieveeditor.renamecategories sieveeditor.categories DESTINATION ${KDE_INSTALL_CONFDIR} )

add_subdirectory(doc)
feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)

