set(sieveeditorimportwizardtest_SRCS main.cpp ${sieveeditor_importimapsettings_SRCS} ${sieveeditor_importimapsettings_check_SRCS})

add_executable(sieveeditorimportwizardtest ${sieveeditorimportwizardtest_SRCS})

target_link_libraries(sieveeditorimportwizardtest
    Qt5::Widgets
    KF5::WidgetsAddons
    KF5::I18n
    KF5::ConfigCore
    )

