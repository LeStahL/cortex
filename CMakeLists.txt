cmake_minimum_required(VERSION 3.8)

project(cortex)

find_package(Python3 REQUIRED COMPONENTS Interpreter)
find_package(OpenGL REQUIRED COMPONENTS OpenGL)

execute_process(
    COMMAND ${Python3_EXECUTABLE} "${PROJECT_SOURCE_DIR}/generator/generator.py" "${PROJECT_SOURCE_DIR}/config/demo.json"
    WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
)
include(config.gen.cmake)

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

set(${DEMO_IDENTIFIER}_SOURCES
    main.cpp
    
    OpenGL.cpp
    Configuration.cpp
    Window.cpp
    Shader.cpp
    SymbolTable.cpp
)

add_executable(${DEMO_IDENTIFIER} ${${DEMO_IDENTIFIER}_SOURCES})
target_link_libraries(${DEMO_IDENTIFIER} OpenGL::GL)

if(MSVC)
    set_target_properties(${DEMO_IDENTIFIER} PROPERTIES LINK_FLAGS "/ENTRY:demo /SUBSYSTEM:Windows /OPT:REF /OPT:ICF=10 /VERBOSE /NODEFAULTLIB")
    target_compile_options(${DEMO_IDENTIFIER} PRIVATE /Gy /O1 /fp:fast /GR- /GS- /MT /d2FH4)
    target_link_libraries(${DEMO_IDENTIFIER} User32 Winmm ucrt msvcrt vcruntime Vfw32)
endif()

if(${CMAKE_BUILD_TYPE} MATCHES "Release")
    find_program(KKRUNCHY_K7 kkrunchy_k7.exe HINTS "${PROJECT_BINARY_DIR}" "${PROJECT_SOURCE_DIR}/bin")
    add_custom_command(TARGET ${DEMO_IDENTIFIER} COMMAND "$<$<CONFIG:RELEASE>:${KKRUNCHY_K7}>" ARGS --best "$<TARGET_FILE:${DEMO_IDENTIFIER}>" --out "$<TARGET_FILE_DIR:${DEMO_IDENTIFIER}>/${DEMO_IDENTIFIER}c.exe" POST_BUILD)
endif()
