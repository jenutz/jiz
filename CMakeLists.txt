cmake_minimum_required(VERSION 3.16)

project(jiz VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Sql Charts Gui)
qt_standard_project_setup()

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(UI_DIR "${CMAKE_CURRENT_SOURCE_DIR}/ui")

# Add the ui directory to the UIC search paths
set(CMAKE_AUTOUIC_SEARCH_PATHS ${UI_DIR})

#add_compile_options(
#  "-Wall" "-Wpedantic" "-Wextra" "-fexceptions"
#  "-g"
#)

file(GLOB CPP_FILES "${SRC_DIR}/*.cpp")
file(GLOB UI_FILES "${UI_DIR}/*.ui")
message(${UI_FILES})
# Add the executable target
qt_add_executable(jiz
    MANUAL_FINALIZATION
    ${CPP_FILES}
    ${UI_FILES}
)

target_include_directories(jiz PRIVATE 
    ${SRC_DIR}
    ${UI_DIR}
)

target_link_libraries(jiz PRIVATE 
    Qt6::Core
    Qt6::Widgets 
    Qt6::Sql 
    Qt6::Charts
    Qt6::Gui
)

qt_finalize_executable(jiz)

