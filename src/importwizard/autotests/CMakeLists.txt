macro(add_sieveeditor_importwizard_unittest _source)
    ecm_add_test(${_source}
        NAME_PREFIX "sieveeditor-importwizard-"
        LINK_LIBRARIES Qt5::Test KF5::I18n Qt5::Gui
    )
endmacro()

add_sieveeditor_importwizard_unittest(importimapsettingsearchpagetest.cpp)
add_sieveeditor_importwizard_unittest(importimapsettingnofoundpagetest.cpp)
add_sieveeditor_importwizard_unittest(importimapsettingsakonadicheckjobtest.cpp)
