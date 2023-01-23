# SPDX-FileCopyrightText: 2013-2023 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
add_definitions( -DIMPORTWIZARD_DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/data" )

macro(add_sieveeditor_importwizard_unittest _source _additionsource)
    get_filename_component(_name ${_source} NAME_WE)
    ecm_add_test(${_source} ${_additionsource} ${_name}.h
        TEST_NAME ${_name}
        NAME_PREFIX "sieveeditor-importwizard-"
        LINK_LIBRARIES Qt::Test KF${KF_MAJOR_VERSION}::I18n Qt::Gui Qt::Widgets
        KF5::MailTransport KF${KF_MAJOR_VERSION}::ConfigCore KF${KF_MAJOR_VERSION}::ConfigGui KF5::KSieveUi libsieveeditor
        )
endmacro()



add_sieveeditor_importwizard_unittest(importimapsettingsearchpagetest.cpp "")
add_sieveeditor_importwizard_unittest(importimapsettingnofoundpagetest.cpp "")
add_sieveeditor_importwizard_unittest(importimapsettingsakonadicheckjobtest.cpp "importimapsettingspasswordtest.cpp")
add_sieveeditor_importwizard_unittest(importimapsettingprogresspagetest.cpp "")
add_sieveeditor_importwizard_unittest(importimapsettingfinishpagetest.cpp "")
add_sieveeditor_importwizard_unittest(importimapsettingsthunderbirdcheckjobtest.cpp "")
