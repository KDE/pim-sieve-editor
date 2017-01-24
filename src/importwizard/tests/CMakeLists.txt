
set(sieveeditor_test_common_SRCS "../../sieveeditorutil.cpp;../../sieveserversettings.cpp")

set(sieveeditorimportwizardtest_SRCS main.cpp ${sieveeditor_importimapsettings_SRCS} ${sieveeditor_importimapsettings_check_SRCS} ${sieveeditor_test_common_SRCS})
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
    )

