project(KubeECS)

get_filename_component(KubeECSDir ${CMAKE_CURRENT_LIST_FILE} PATH)

set(KubeECSSources
    ${KubeECSDir}/Base.hpp
    ${KubeECSDir}/Entity.hpp
    ${KubeECSDir}/Component.hpp
    ${KubeECSDir}/Component.ipp
    ${KubeECSDir}/Instance.hpp
    ${KubeECSDir}/Instance.ipp
    ${KubeECSDir}/Instance.cpp
    ${KubeECSDir}/ComponentHolder.hpp
    ${KubeECSDir}/EntityHolder.hpp
    ${KubeECSDir}/EntityHolder.ipp
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