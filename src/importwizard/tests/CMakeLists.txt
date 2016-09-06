set(sieveeditorimportwizardtest_SRCS main.cpp)

add_executable(sieveeditorimportwizardtest ${sieveeditorimportwizardtest_SRCS})

target_link_libraries(sieveeditorimportwizardtest
    Qt5::Widgets
    )

