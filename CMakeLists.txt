cmake_minimum_required(VERSION 3.8)

project(cortex)

set(${PROJECT_NAME}_SOURCES
    main.cpp
    
    Configuration.cpp
    Window.cpp
)

find_package(Python3 REQUIRED COMPONENTS Interpreter)
find_package(OpenGL REQUIRED COMPONENTS OpenGL)

if(MSVC)
    set(PLATFORM_DEFINITION "#define MSVC")
elseif(UNIX)
    set(PLATFORM_DEFINITION "#define UNIX")
endif()

set(BUILD_TYPE_DEFINITION "#undef DEBUG")
if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
    set(BUILD_TYPE_DEFINITION "#define DEBUG")
endif()

configure_file(Build.hpp.in Build.gen.hpp)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_SOURCES})
target_link_libraries(${PROJECT_NAME} OpenGL::GL)

if(MSVC)
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/ENTRY:demo /SUBSYSTEM:Windows /OPT:REF /OPT:ICF=10 /VERBOSE /NODEFAULTLIB")
    target_compile_options(${PROJECT_NAME} PRIVATE /Gy /O1 /fp:fast /GR- /GS- /MT /d2FH4)
    target_link_libraries(${PROJECT_NAME} User32 Winmm ucrt msvcrt vcruntime Vfw32)
endif()

