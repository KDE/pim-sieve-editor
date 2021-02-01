
set(sieveeditorimportwizardtest_SRCS main.cpp)

add_executable(sieveeditorimportwizardtest ${sieveeditorimportwizardtest_SRCS})

target_link_libraries(sieveeditorimportwizardtest
    Qt5::Widgets
    KF5::WidgetsAddons
    KF5::I18n
    KF5::ConfigCore
    KF5::ConfigGui
    KF5::MailTransport
    KF5::KSieveUi
    libsieveeditor
    )

