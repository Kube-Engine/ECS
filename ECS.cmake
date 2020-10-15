project(KubeECS)

get_filename_component(KubeECSDir ${CMAKE_CURRENT_LIST_FILE} PATH)

set(KubeECSSources
    ${KubeECSDir}/Dummy.cpp
)

add_library(${PROJECT_NAME} ${KubeECSSources})

target_link_libraries(${PROJECT_NAME}
PUBLIC
    KubeCore
)

if(${KF_TESTS})
    include(${KubeECSDir}/Tests/ECSTests.cmake)
endif()

if(${KF_BENCHMARKS})
    include(${KubeECSDir}/Benchmarks/ECSBenchmarks.cmake)
endif()