cmake_minimum_required(VERSION 3.8)

project(cortex)

set(${PROJECT_NAME}_SOURCES
    main.cpp
    
    Window.cpp
)

find_package(Python3 REQUIRED COMPONENTS Interpreter)
find_package(OpenGL REQUIRED COMPONENTS OpenGL)

add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_SOURCES})
target_link_libraries(${PROJECT_NAME} OpenGL::GL)

if(MSVC)
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/ENTRY:demo /SUBSYSTEM:Windows /OPT:REF /OPT:ICF=10 /VERBOSE /NODEFAULTLIB")
    target_compile_options(${PROJECT_NAME} PRIVATE /Gy /O1 /fp:fast /GR- /GS- /MT /d2FH4)
    target_link_libraries(${PROJECT_NAME} User32 Winmm ucrt msvcrt vcruntime Vfw32)
endif()
