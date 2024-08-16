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
