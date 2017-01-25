add_definitions( -DIMPORTWIZARD_DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/data" )

set(sieveeditor_autotest_common_SRCS)
ecm_qt_declare_logging_category(sieveeditor_autotest_common_SRCS HEADER sieveeditor_debug.h IDENTIFIER SIEVEEDITOR_LOG CATEGORY_NAME org.kde.pim.sieveeditor)

macro(add_sieveeditor_importwizard_unittest _source _additionalsource)
    get_filename_component(_name ${_source} NAME_WE)
    ecm_add_test(${_source} ${_additionalsource} ${sieveeditor_autotest_common_SRCS}
        TEST_NAME ${_name}
        NAME_PREFIX "sieveeditor-importwizard-"
        LINK_LIBRARIES Qt5::Test KF5::I18n Qt5::Gui Qt5::Widgets KF5::MailTransport KF5::ConfigCore KF5::ConfigGui KF5::KSieveUi KF5::Wallet sieveeditorlib
    )
endmacro()



add_sieveeditor_importwizard_unittest(importimapsettingsearchpagetest.cpp "../importimapsettingsearchpage.cpp")
add_sieveeditor_importwizard_unittest(importimapsettingnofoundpagetest.cpp "../importimapsettingnofoundpage.cpp")
add_sieveeditor_importwizard_unittest(importimapsettingsakonadicheckjobtest.cpp "../checks/importimapsettingsakonadicheckjob.cpp;../checks/abstractimapsettingscheckjob.cpp")
add_sieveeditor_importwizard_unittest(importimapsettingprogresspagetest.cpp "../importimapsettingprogresspage.cpp;../checks/importimapsettingsthunderbirdcheckjob.cpp;../checks/abstractimapsettingscheckjob.cpp")
add_sieveeditor_importwizard_unittest(importimapsettingfinishpagetest.cpp "../importimapsettingfinishpage.cpp")
add_sieveeditor_importwizard_unittest(importimapsettingsthunderbirdcheckjobtest.cpp "../checks/importimapsettingsthunderbirdcheckjob.cpp;../checks/abstractimapsettingscheckjob.cpp")
