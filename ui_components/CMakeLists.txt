cmake_minimum_required(VERSION 3.10)
project(ui_components)

add_definitions(-DSUPPORT_CEF)

include_directories(${CMAKE_CURRENT_LIST_DIR})
include_directories(${CMAKE_CURRENT_LIST_DIR}/../)
include_directories(${CMAKE_CURRENT_LIST_DIR}/../third_party/cef_wrapper/)

source_group("" FILES
    ${CMAKE_CURRENT_LIST_DIR}/./public_define.h
    ${CMAKE_CURRENT_LIST_DIR}/./stdafx.cpp
    ${CMAKE_CURRENT_LIST_DIR}/./stdafx.h
    ${CMAKE_CURRENT_LIST_DIR}/./targetver.h
    ${CMAKE_CURRENT_LIST_DIR}/./ui_components.h
)

source_group("comboex" FILES
    ${CMAKE_CURRENT_LIST_DIR}/comboex/CheckCombo.cpp
    ${CMAKE_CURRENT_LIST_DIR}/comboex/CheckCombo.h
    ${CMAKE_CURRENT_LIST_DIR}/comboex/FilterCombo.cpp
    ${CMAKE_CURRENT_LIST_DIR}/comboex/FilterCombo.h
    ${CMAKE_CURRENT_LIST_DIR}/comboex/ShadowCombo.cpp
    ${CMAKE_CURRENT_LIST_DIR}/comboex/ShadowCombo.h
)

source_group("menu" FILES
    ${CMAKE_CURRENT_LIST_DIR}/menu/observer_impl_base.hpp
    ${CMAKE_CURRENT_LIST_DIR}/menu/ui_menu.cpp
    ${CMAKE_CURRENT_LIST_DIR}/menu/ui_menu.h
)

source_group("modal_wnd" FILES
    ${CMAKE_CURRENT_LIST_DIR}/modal_wnd/async_do_modal.cpp
    ${CMAKE_CURRENT_LIST_DIR}/modal_wnd/async_do_modal.h
    ${CMAKE_CURRENT_LIST_DIR}/modal_wnd/async_modal_runner.cpp
    ${CMAKE_CURRENT_LIST_DIR}/modal_wnd/async_modal_runner.h
    ${CMAKE_CURRENT_LIST_DIR}/modal_wnd/file_dialog_ex.cpp
    ${CMAKE_CURRENT_LIST_DIR}/modal_wnd/file_dialog_ex.h
    ${CMAKE_CURRENT_LIST_DIR}/modal_wnd/modal_wnd_base.h
)

source_group("msgbox" FILES
    ${CMAKE_CURRENT_LIST_DIR}/msgbox/msgbox.cpp
    ${CMAKE_CURRENT_LIST_DIR}/msgbox/msgbox.h
)

source_group("popover" FILES
    ${CMAKE_CURRENT_LIST_DIR}/popover/alert.cpp
    ${CMAKE_CURRENT_LIST_DIR}/popover/alert.h
    ${CMAKE_CURRENT_LIST_DIR}/popover/notification.cpp
    ${CMAKE_CURRENT_LIST_DIR}/popover/notification.h
    ${CMAKE_CURRENT_LIST_DIR}/popover/popover.cpp
    ${CMAKE_CURRENT_LIST_DIR}/popover/popover.h
    ${CMAKE_CURRENT_LIST_DIR}/popover/tooltip.cpp
    ${CMAKE_CURRENT_LIST_DIR}/popover/tooltip.h
)

source_group("shadow_wnd" FILES
    ${CMAKE_CURRENT_LIST_DIR}/shadow_wnd/shadow_wnd.cpp
    ${CMAKE_CURRENT_LIST_DIR}/shadow_wnd/shadow_wnd.h
)

source_group("toast" FILES
    ${CMAKE_CURRENT_LIST_DIR}/toast/toast.cpp
    ${CMAKE_CURRENT_LIST_DIR}/toast/toast.h
)

source_group("windows_manager" FILES
    ${CMAKE_CURRENT_LIST_DIR}/windows_manager/windows_manager.cpp
    ${CMAKE_CURRENT_LIST_DIR}/windows_manager/windows_manager.h
    ${CMAKE_CURRENT_LIST_DIR}/windows_manager/window_ex.cpp
    ${CMAKE_CURRENT_LIST_DIR}/windows_manager/window_ex.h
)

set(SOURCE_FILES
    ${CMAKE_CURRENT_LIST_DIR}/./public_define.h
    ${CMAKE_CURRENT_LIST_DIR}/./stdafx.cpp
    ${CMAKE_CURRENT_LIST_DIR}/./stdafx.h
    ${CMAKE_CURRENT_LIST_DIR}/./targetver.h
    ${CMAKE_CURRENT_LIST_DIR}/./ui_components.h
    ${CMAKE_CURRENT_LIST_DIR}/comboex/CheckCombo.cpp
    ${CMAKE_CURRENT_LIST_DIR}/comboex/CheckCombo.h
    ${CMAKE_CURRENT_LIST_DIR}/comboex/FilterCombo.cpp
    ${CMAKE_CURRENT_LIST_DIR}/comboex/FilterCombo.h
    ${CMAKE_CURRENT_LIST_DIR}/comboex/ShadowCombo.cpp
    ${CMAKE_CURRENT_LIST_DIR}/comboex/ShadowCombo.h
    ${CMAKE_CURRENT_LIST_DIR}/menu/observer_impl_base.hpp
    ${CMAKE_CURRENT_LIST_DIR}/menu/ui_menu.cpp
    ${CMAKE_CURRENT_LIST_DIR}/menu/ui_menu.h
    ${CMAKE_CURRENT_LIST_DIR}/modal_wnd/async_do_modal.cpp
    ${CMAKE_CURRENT_LIST_DIR}/modal_wnd/async_do_modal.h
    ${CMAKE_CURRENT_LIST_DIR}/modal_wnd/async_modal_runner.cpp
    ${CMAKE_CURRENT_LIST_DIR}/modal_wnd/async_modal_runner.h
    ${CMAKE_CURRENT_LIST_DIR}/modal_wnd/file_dialog_ex.cpp
    ${CMAKE_CURRENT_LIST_DIR}/modal_wnd/file_dialog_ex.h
    ${CMAKE_CURRENT_LIST_DIR}/modal_wnd/modal_wnd_base.h
    ${CMAKE_CURRENT_LIST_DIR}/msgbox/msgbox.cpp
    ${CMAKE_CURRENT_LIST_DIR}/msgbox/msgbox.h
    ${CMAKE_CURRENT_LIST_DIR}/popover/alert.cpp
    ${CMAKE_CURRENT_LIST_DIR}/popover/alert.h
    ${CMAKE_CURRENT_LIST_DIR}/popover/notification.cpp
    ${CMAKE_CURRENT_LIST_DIR}/popover/notification.h
    ${CMAKE_CURRENT_LIST_DIR}/popover/popover.cpp
    ${CMAKE_CURRENT_LIST_DIR}/popover/popover.h
    ${CMAKE_CURRENT_LIST_DIR}/popover/tooltip.cpp
    ${CMAKE_CURRENT_LIST_DIR}/popover/tooltip.h
    ${CMAKE_CURRENT_LIST_DIR}/shadow_wnd/shadow_wnd.cpp
    ${CMAKE_CURRENT_LIST_DIR}/shadow_wnd/shadow_wnd.h
    ${CMAKE_CURRENT_LIST_DIR}/toast/toast.cpp
    ${CMAKE_CURRENT_LIST_DIR}/toast/toast.h
    ${CMAKE_CURRENT_LIST_DIR}/windows_manager/windows_manager.cpp
    ${CMAKE_CURRENT_LIST_DIR}/windows_manager/windows_manager.h
    ${CMAKE_CURRENT_LIST_DIR}/windows_manager/window_ex.cpp
    ${CMAKE_CURRENT_LIST_DIR}/windows_manager/window_ex.h
)

add_library(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} base)
target_link_libraries(${PROJECT_NAME} duilib)
