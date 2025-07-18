# SPDX-FileCopyrightText: 2013-2025 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
add_definitions(-DIMPORTWIZARD_DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/data")

macro(add_sieveeditor_importwizard_unittest _source _additionsource)
    get_filename_component(_name ${_source} NAME_WE)
    ecm_add_test(${_source} ${_additionsource} ${_name}.h
        TEST_NAME ${_name}
        NAME_PREFIX "sieveeditor-importwizard-"
        LINK_LIBRARIES Qt::Test KF6::I18n Qt::Gui Qt::Widgets
        KPim6::MailTransport KF6::ConfigCore KF6::ConfigGui KPim6::KSieveUi libsieveeditor
    )
endmacro()

add_sieveeditor_importwizard_unittest(importimapsettingsearchpagetest.cpp "")
add_sieveeditor_importwizard_unittest(importimapsettingnofoundpagetest.cpp "")
add_sieveeditor_importwizard_unittest(importimapsettingsakonadicheckjobtest.cpp "importimapsettingspasswordtest.cpp")
add_sieveeditor_importwizard_unittest(importimapsettingprogresspagetest.cpp "")
add_sieveeditor_importwizard_unittest(importimapsettingfinishpagetest.cpp "")
add_sieveeditor_importwizard_unittest(importimapsettingsthunderbirdcheckjobtest.cpp "")
