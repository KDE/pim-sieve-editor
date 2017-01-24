macro(add_sieveeditor_importwizard_unittest _source _additionalsource)
    get_filename_component(_name ${_source} NAME_WE)
    ecm_add_test(${_source} ${_additionalsource}
        TEST_NAME ${_name}
        NAME_PREFIX "sieveeditor-importwizard-"
        LINK_LIBRARIES Qt5::Test KF5::I18n Qt5::Gui Qt5::Widgets
    )
endmacro()



add_sieveeditor_importwizard_unittest(importimapsettingsearchpagetest.cpp "../importimapsettingsearchpage.cpp")
add_sieveeditor_importwizard_unittest(importimapsettingnofoundpagetest.cpp "../importimapsettingnofoundpage.cpp")
add_sieveeditor_importwizard_unittest(importimapsettingsakonadicheckjobtest.cpp "../checks/importimapsettingsakonadicheckjob.cpp;../checks/abstractimapsettingscheckjob.cpp")
add_sieveeditor_importwizard_unittest(importimapsettingprogresspagetest.cpp "../importimapsettingprogresspage.cpp")
add_sieveeditor_importwizard_unittest(importimapsettingfinishpagetest.cpp "../importimapsettingfinishpage.cpp")
add_sieveeditor_importwizard_unittest(importimapsettingsthunderbirdcheckjobtest.cpp "../checks/importimapsettingsthunderbirdcheckjob.cpp;../checks/abstractimapsettingscheckjob.cpp")
