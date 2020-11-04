project(KubeECS)

get_filename_component(KubeECSDir ${CMAKE_CURRENT_LIST_FILE} PATH)

set(KubeECSSources
    ${KubeECSDir}/Dummy.cpp
    ${KubeECSDir}/Base.hpp
    ${KubeECSDir}/SparseEntitySet.hpp
    ${KubeECSDir}/SparseEntitySet.ipp
    ${KubeECSDir}/ComponentTable.hpp
    ${KubeECSDir}/ComponentTable.ipp
    ${KubeECSDir}/ComponentTables.hpp
    ${KubeECSDir}/ComponentTables.ipp
    ${KubeECSDir}/ASystem.hpp
    ${KubeECSDir}/Registry.hpp
    ${KubeECSDir}/SystemGraph.ipp
    ${KubeECSDir}/SystemGraph.hpp
    ${KubeECSDir}/Registry.ipp
)

add_library(${PROJECT_NAME} ${KubeECSSources})

target_link_libraries(${PROJECT_NAME}
PUBLIC
    KubeCore
    KubeMeta
    KubeFlow
)

if(${KF_TESTS})
    include(${KubeECSDir}/Tests/ECSTests.cmake)
endif()

if(${KF_BENCHMARKS})
    include(${KubeECSDir}/Benchmarks/ECSBenchmarks.cmake)
endif()