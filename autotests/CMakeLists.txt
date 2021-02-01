
add_executable( serversievesettingstest  serversievesettingstest.cpp)
add_test(NAME serversievesettingstest COMMAND serversievesettingstest)
ecm_mark_as_test(serversievesettingstest)
target_link_libraries( serversievesettingstest Qt5::Test Qt5::Widgets KF5::MailTransport KF5::I18n KF5::ConfigGui KF5::KSieveUi libsieveeditor)

set( sieveeditorutiltest_source sieveeditorutiltest.cpp )
add_executable( sieveeditorutiltest ${sieveeditorutiltest_source})
add_test(NAME sieveeditorutiltest COMMAND sieveeditorutiltest)
ecm_mark_as_test(sieveeditorutiltest)
target_link_libraries( sieveeditorutiltest Qt5::Test KF5::MailTransport KF5::I18n KF5::ConfigGui KF5::KSieveUi libsieveeditor)

# convenience macro to add qtest unit tests
macro(add_sieveserver_unittest _source _additionalSource)
    set(_test ${_source} ${_additionalSource})
    get_filename_component(_name ${_source} NAME_WE)
    add_executable( ${_name} ${_test} )
    add_test( NAME ${_name} COMMAND ${_name} )
    ecm_mark_as_test(sieveserver-${_name})
    target_link_libraries( ${_name}
        Qt5::Test
        Qt5::Widgets
        KF5::I18n
        KF5::MailTransport
        KF5::KSieveUi
        libsieveeditor
        )
endmacro ()

add_sieveserver_unittest( sieveeditorconfigureserverpagetest.cpp "")

