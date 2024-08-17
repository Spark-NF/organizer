project(cli)

# Qt libraries
find_package(Qt6 COMPONENTS Core Network REQUIRED)
set(QT_LIBRARIES Qt6::Core Qt6::Network)

# Input
file(GLOB_RECURSE SOURCES "src/*.cpp")
include_directories("src/" "../lib/src")

# Target
add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${QT_LIBRARIES} lib)
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "Organizer-cli")

# Install
if(UNIX AND (NOT APPLE))
	install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
endif()

# Tests
file(GLOB_RECURSE SOURCES_TESTS "src/*.cpp" "tests/*.cpp")
list(REMOVE_ITEM SOURCES_TESTS "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")
include_directories("src/" "../vendor" "../vendor/catch/src")
add_executable(${PROJECT_NAME}-tests ${SOURCES_TESTS})
add_test(${PROJECT_NAME}-tests ${PROJECT_NAME}-tests)
find_package(Qt6 COMPONENTS Test REQUIRED)
target_link_libraries(${PROJECT_NAME}-tests ${QT_LIBRARIES} Qt6::Test lib Catch2::Catch2)
add_dependencies(${PROJECT_NAME}-tests ${PROJECT_NAME})
