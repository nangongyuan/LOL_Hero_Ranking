cmake_minimum_required(VERSION 3.10)

# set(CMAKE_EXE_LINKER_FLAGS"${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")

project(examples)

add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

add_definitions(-DUNICODE -D_UNICODE)

add_subdirectory(base)
add_subdirectory(duilib)
add_subdirectory(ui_components)
add_subdirectory(phlib)
add_subdirectory(examples/LOL)


set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMakeTargets")

set_target_properties(LOL PROPERTIES FOLDER "examples")









