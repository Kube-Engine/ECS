project(KubeECSBenchmarks)

get_filename_component(KubeECSBenchmarksDir ${CMAKE_CURRENT_LIST_FILE} PATH)

set(KubeECSBenchmarksSources
    ${KubeECSBenchmarksDir}/Main.cpp
)

add_executable(${CMAKE_PROJECT_NAME} ${KubeECSBenchmarksSources})

target_link_libraries(${CMAKE_PROJECT_NAME}
PUBLIC
    KubeECS
    benchmark::benchmark
)