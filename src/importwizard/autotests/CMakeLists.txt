add_definitions( -DIMPORTWIZARD_DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/data" )

macro(add_sieveeditor_importwizard_unittest _source)
    get_filename_component(_name ${_source} NAME_WE)
    ecm_add_test(${_source}
        TEST_NAME ${_name}
        NAME_PREFIX "sieveeditor-importwizard-"
        LINK_LIBRARIES Qt5::Test KF5::I18n Qt5::Gui Qt5::Widgets KF5::MailTransport KF5::ConfigCore KF5::ConfigGui KF5::KSieveUi KF5::Wallet libsieveeditor
        )
endmacro()



add_sieveeditor_importwizard_unittest(importimapsettingsearchpagetest.cpp)
add_sieveeditor_importwizard_unittest(importimapsettingnofoundpagetest.cpp)
add_sieveeditor_importwizard_unittest(importimapsettingsakonadicheckjobtest.cpp)
add_sieveeditor_importwizard_unittest(importimapsettingprogresspagetest.cpp)
add_sieveeditor_importwizard_unittest(importimapsettingfinishpagetest.cpp)
add_sieveeditor_importwizard_unittest(importimapsettingsthunderbirdcheckjobtest.cpp)
