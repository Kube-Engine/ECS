project(KubeECSTests)

get_filename_component(KubeECSTestsDir ${CMAKE_CURRENT_LIST_FILE} PATH)

set(KubeECSTestsSources
    ${KubeECSTestsDir}/tests_SparseEntitySet.cpp
    ${KubeECSTestsDir}/tests_ComponentTable.cpp
    ${KubeECSTestsDir}/tests_ComponentTables.cpp
    ${KubeECSTestsDir}/tests_Registry.cpp
    ${KubeECSTestsDir}/tests_View.cpp
    ${KubeECSTestsDir}/tests_SystemGraph.cpp
    ${KubeECSTestsDir}/tests.cpp
)

add_executable(${CMAKE_PROJECT_NAME} ${KubeECSTestsSources})

add_test(NAME ${CMAKE_PROJECT_NAME} COMMAND ${CMAKE_PROJECT_NAME})

target_link_libraries(${CMAKE_PROJECT_NAME}
PUBLIC
    KubeECS
    GTest::GTest GTest::Main
)

if(KF_COVERAGE)
    target_compile_options(${PROJECT_NAME} PUBLIC --coverage)
    target_link_options(${PROJECT_NAME} PUBLIC --coverage)
endif()