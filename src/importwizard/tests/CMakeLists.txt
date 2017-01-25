
set(sieveeditorimportwizardtest_SRCS main.cpp)
ecm_qt_declare_logging_category(sieveeditorimportwizardtest_SRCS HEADER sieveeditor_debug.h IDENTIFIER SIEVEEDITOR_LOG CATEGORY_NAME org.kde.pim.sieveeditor)


add_executable(sieveeditorimportwizardtest ${sieveeditorimportwizardtest_SRCS})

target_link_libraries(sieveeditorimportwizardtest
    Qt5::Widgets
    KF5::WidgetsAddons
    KF5::I18n
    KF5::ConfigCore
    KF5::ConfigGui
    KF5::MailTransport
    KF5::KSieveUi
    sieveeditorlib
    )

