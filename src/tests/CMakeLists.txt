project(tests)

# Qt libraries
find_package(Qt6 COMPONENTS Core Gui Network REQUIRED)
set(QT_LIBRARIES Qt6::Core Qt6::Gui Qt6::Network)

# Input
file(GLOB_RECURSE SOURCES "src/*.cpp")
include_directories("src/")

# Target
add_library(${PROJECT_NAME} STATIC ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${QT_LIBRARIES})
