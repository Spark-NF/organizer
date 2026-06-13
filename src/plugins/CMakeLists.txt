project(plugins)

# Qt libraries
find_package(Qt6 COMPONENTS Core REQUIRED)

# Install
if(UNIX AND NOT APPLE)
    install(DIRECTORY "src/" DESTINATION "share/organizer/plugins")
endif()

# Tests
file(GLOB_RECURSE SOURCES_TESTS "tests/*.cpp")
include_directories("../lib/src/" "../tests/src/" "../vendor" "../vendor/catch/src")
add_executable(${PROJECT_NAME}-tests ${SOURCES_TESTS})
add_test(${PROJECT_NAME}-tests ${PROJECT_NAME}-tests)
find_package(Qt6 COMPONENTS Test REQUIRED)
target_link_libraries(${PROJECT_NAME}-tests Qt6::Core Qt6::Test lib tests Catch2::Catch2)
target_compile_definitions(${PROJECT_NAME}-tests PRIVATE "PLUGINS_DIR=\"${CMAKE_CURRENT_SOURCE_DIR}/src\"")
