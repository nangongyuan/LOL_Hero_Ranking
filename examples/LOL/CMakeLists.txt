cmake_minimum_required(VERSION 3.10)

project(LOL)

# 设置 C++17 标准
set(CMAKE_CXX_STANDARD 17)

add_definitions(-DUNICODE -D_UNICODE)


include_directories(${CMAKE_CURRENT_LIST_DIR})
include_directories(${CMAKE_CURRENT_LIST_DIR}/../../)
include_directories(${CMAKE_CURRENT_LIST_DIR}/../../ui_components)

include_directories(${CMAKE_CURRENT_LIST_DIR}/../../vcpkg_installed/x64-windows-static-md/include)

source_group("" FILES
    ${CMAKE_CURRENT_LIST_DIR}/./OPGGRequest.cpp
    ${CMAKE_CURRENT_LIST_DIR}/./OPGGRequest.h
    ${CMAKE_CURRENT_LIST_DIR}/./LCURequest.cpp
    ${CMAKE_CURRENT_LIST_DIR}/./LCURequest.h
    ${CMAKE_CURRENT_LIST_DIR}/./LOLGame.cpp
    ${CMAKE_CURRENT_LIST_DIR}/./LOLGame.h
    ${CMAKE_CURRENT_LIST_DIR}/./MainWindow.cpp
    ${CMAKE_CURRENT_LIST_DIR}/./MainWindow.h
    ${CMAKE_CURRENT_LIST_DIR}/./main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/./main.h
    ${CMAKE_CURRENT_LIST_DIR}/./resource.h
    ${CMAKE_CURRENT_LIST_DIR}/./stdafx.cpp
    ${CMAKE_CURRENT_LIST_DIR}/./stdafx.h
    ${CMAKE_CURRENT_LIST_DIR}/./targetver.h
    ${CMAKE_CURRENT_LIST_DIR}/./Common.h
)

set(SOURCE_FILES
    ${CMAKE_CURRENT_LIST_DIR}/./OPGGRequest.cpp
    ${CMAKE_CURRENT_LIST_DIR}/./OPGGRequest.h
    ${CMAKE_CURRENT_LIST_DIR}/./LCURequest.cpp
    ${CMAKE_CURRENT_LIST_DIR}/./LCURequest.h
    ${CMAKE_CURRENT_LIST_DIR}/./LOLGame.cpp
    ${CMAKE_CURRENT_LIST_DIR}/./LOLGame.h
    ${CMAKE_CURRENT_LIST_DIR}/./MainWindow.cpp
    ${CMAKE_CURRENT_LIST_DIR}/./MainWindow.h
    ${CMAKE_CURRENT_LIST_DIR}/./main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/./main.h
    ${CMAKE_CURRENT_LIST_DIR}/./resource.h
    ${CMAKE_CURRENT_LIST_DIR}/./stdafx.cpp
    ${CMAKE_CURRENT_LIST_DIR}/./stdafx.h
    ${CMAKE_CURRENT_LIST_DIR}/./targetver.h
    ${CMAKE_CURRENT_LIST_DIR}/./Common.h
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

add_dependencies(${PROJECT_NAME} base duilib)
target_link_libraries(${PROJECT_NAME} base)
target_link_libraries(${PROJECT_NAME} duilib)
target_link_libraries(${PROJECT_NAME} ui_components)


set(LIBRARY_PATH_DEBUG ${CMAKE_CURRENT_LIST_DIR}/../../vcpkg_installed/x64-windows-static-md/debug/lib)
set(LIBRARY_PATH_RELEASE ${CMAKE_CURRENT_LIST_DIR}/../../vcpkg_installed/x64-windows-static-md/lib)


# 链接库
# 区分 Debug 和 Release 版本的库
target_link_libraries(${PROJECT_NAME}
  debug ${LIBRARY_PATH_DEBUG}/cpr.lib ${LIBRARY_PATH_DEBUG}/fmtd.lib ${LIBRARY_PATH_DEBUG}/libcurl-d.lib ${LIBRARY_PATH_DEBUG}/spdlogd.lib ${LIBRARY_PATH_DEBUG}/zlibd.lib ${LIBRARY_PATH_DEBUG}/jsoncpp.lib Ws2_32.lib Crypt32.lib
#  optimized ${LIBRARY_PATH_RELEASE}/cpr.lib ${LIBRARY_PATH_RELEASE}/fmt.lib ${LIBRARY_PATH_RELEASE}/libcurl.lib ${LIBRARY_PATH_RELEASE}/spdlog.lib ${LIBRARY_PATH_RELEASE}/zlib.lib ${LIBRARY_PATH_RELEASE}/jsoncpp.lib Ws2_32.lib Crypt32.lib
)

set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")


if (MSVC)
    set_target_properties(${PROJECT_NAME}
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_HOME_DIRECTORY}/bin"
        RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_HOME_DIRECTORY}/bin"
    )
endif (MSVC)

