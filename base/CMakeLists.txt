cmake_minimum_required(VERSION 3.10)
project(base)

add_definitions(-DUNICODE -D_UNICODE)

include_directories(${CMAKE_CURRENT_LIST_DIR}/../)

source_group("" FILES
    ${CMAKE_CURRENT_LIST_DIR}/./base.h
    ${CMAKE_CURRENT_LIST_DIR}/./base_config.h
    ${CMAKE_CURRENT_LIST_DIR}/./base_export.h
    ${CMAKE_CURRENT_LIST_DIR}/./base_types.h
    ${CMAKE_CURRENT_LIST_DIR}/./compiler_specific.h
    ${CMAKE_CURRENT_LIST_DIR}/./error.h
    ${CMAKE_CURRENT_LIST_DIR}/./macros.h
    ${CMAKE_CURRENT_LIST_DIR}/./port.h
)

source_group("callback" FILES
    ${CMAKE_CURRENT_LIST_DIR}/callback/callback.h
)

source_group("encrypt" FILES
    ${CMAKE_CURRENT_LIST_DIR}/encrypt/des.cpp
    ${CMAKE_CURRENT_LIST_DIR}/encrypt/des.h
    ${CMAKE_CURRENT_LIST_DIR}/encrypt/encrypt.h
    ${CMAKE_CURRENT_LIST_DIR}/encrypt/encrypt_impl.h
)

source_group("file" FILES
    ${CMAKE_CURRENT_LIST_DIR}/file/file_path.h
    ${CMAKE_CURRENT_LIST_DIR}/file/file_util.cpp
    ${CMAKE_CURRENT_LIST_DIR}/file/file_util.h
    ${CMAKE_CURRENT_LIST_DIR}/file/file_util_posix.cpp
    ${CMAKE_CURRENT_LIST_DIR}/file/file_util_win.cpp
)

source_group("framework" FILES
    ${CMAKE_CURRENT_LIST_DIR}/framework/default_message_pump.cpp
    ${CMAKE_CURRENT_LIST_DIR}/framework/default_message_pump.h
    ${CMAKE_CURRENT_LIST_DIR}/framework/message_loop.cpp
    ${CMAKE_CURRENT_LIST_DIR}/framework/message_loop.h
    ${CMAKE_CURRENT_LIST_DIR}/framework/message_loop_proxy.cpp
    ${CMAKE_CURRENT_LIST_DIR}/framework/message_loop_proxy.h
    ${CMAKE_CURRENT_LIST_DIR}/framework/message_pump.cpp
    ${CMAKE_CURRENT_LIST_DIR}/framework/message_pump.h
    ${CMAKE_CURRENT_LIST_DIR}/framework/observer_list.h
    ${CMAKE_CURRENT_LIST_DIR}/framework/task.h
    ${CMAKE_CURRENT_LIST_DIR}/framework/win_io_message_pump.cpp
    ${CMAKE_CURRENT_LIST_DIR}/framework/win_io_message_pump.h
    ${CMAKE_CURRENT_LIST_DIR}/framework/win_message_pump.cpp
    ${CMAKE_CURRENT_LIST_DIR}/framework/win_message_pump.h
    ${CMAKE_CURRENT_LIST_DIR}/framework/win_ui_message_pump.cpp
    ${CMAKE_CURRENT_LIST_DIR}/framework/win_ui_message_pump.h
)

source_group("hardware" FILES
    ${CMAKE_CURRENT_LIST_DIR}/hardware/cpu.cpp
    ${CMAKE_CURRENT_LIST_DIR}/hardware/cpu.h
)

source_group("memory" FILES
    ${CMAKE_CURRENT_LIST_DIR}/memory/blockbuffer.h
    ${CMAKE_CURRENT_LIST_DIR}/memory/deleter.h
    ${CMAKE_CURRENT_LIST_DIR}/memory/lazy_instance.h
    ${CMAKE_CURRENT_LIST_DIR}/memory/singleton.h
)

source_group("network" FILES
    ${CMAKE_CURRENT_LIST_DIR}/network/network_util.cpp
    ${CMAKE_CURRENT_LIST_DIR}/network/network_util.h
    ${CMAKE_CURRENT_LIST_DIR}/network/nio_base.h
)

source_group("synchronization" FILES
    ${CMAKE_CURRENT_LIST_DIR}/synchronization/lock.h
    ${CMAKE_CURRENT_LIST_DIR}/synchronization/lock_posix.cpp
    ${CMAKE_CURRENT_LIST_DIR}/synchronization/lock_win.cpp
    ${CMAKE_CURRENT_LIST_DIR}/synchronization/semaphore.h
    ${CMAKE_CURRENT_LIST_DIR}/synchronization/semaphore_posix.cpp
    ${CMAKE_CURRENT_LIST_DIR}/synchronization/semaphore_win.cpp
    ${CMAKE_CURRENT_LIST_DIR}/synchronization/waitable_event.h
    ${CMAKE_CURRENT_LIST_DIR}/synchronization/waitable_event_posix.cpp
    ${CMAKE_CURRENT_LIST_DIR}/synchronization/waitable_event_win.cpp
)

source_group("third_party/chrome" FILES
    ${CMAKE_CURRENT_LIST_DIR}/third_party/chrome/atomicops.h
    ${CMAKE_CURRENT_LIST_DIR}/third_party/chrome/atomicops_internals_arm_gcc.h
    ${CMAKE_CURRENT_LIST_DIR}/third_party/chrome/atomicops_internals_atomicword_compat.h
    ${CMAKE_CURRENT_LIST_DIR}/third_party/chrome/atomicops_internals_x86_gcc.cc
    ${CMAKE_CURRENT_LIST_DIR}/third_party/chrome/atomicops_internals_x86_gcc.h
    ${CMAKE_CURRENT_LIST_DIR}/third_party/chrome/atomicops_internals_x86_macosx.h
    ${CMAKE_CURRENT_LIST_DIR}/third_party/chrome/atomicops_internals_x86_msvc.h
)

source_group("third_party/convert_utf" FILES
    ${CMAKE_CURRENT_LIST_DIR}/third_party/convert_utf/ConvertUTF.c
    ${CMAKE_CURRENT_LIST_DIR}/third_party/convert_utf/ConvertUTF.h
)

source_group("third_party/modp_b64" FILES
    ${CMAKE_CURRENT_LIST_DIR}/third_party/modp_b64/modp_b64.cc
    ${CMAKE_CURRENT_LIST_DIR}/third_party/modp_b64/modp_b64.h
    ${CMAKE_CURRENT_LIST_DIR}/third_party/modp_b64/modp_b64_data.h
)

source_group("thread" FILES
    ${CMAKE_CURRENT_LIST_DIR}/thread/framework_thread.cpp
    ${CMAKE_CURRENT_LIST_DIR}/thread/framework_thread.h
    ${CMAKE_CURRENT_LIST_DIR}/thread/thread.h
    ${CMAKE_CURRENT_LIST_DIR}/thread/thread_local.h
    ${CMAKE_CURRENT_LIST_DIR}/thread/thread_local_posix.cpp
    ${CMAKE_CURRENT_LIST_DIR}/thread/thread_local_win.cpp
    ${CMAKE_CURRENT_LIST_DIR}/thread/thread_manager.cpp
    ${CMAKE_CURRENT_LIST_DIR}/thread/thread_manager.h
    ${CMAKE_CURRENT_LIST_DIR}/thread/thread_posix.cpp
    ${CMAKE_CURRENT_LIST_DIR}/thread/thread_win.cpp
)

source_group("time" FILES
    ${CMAKE_CURRENT_LIST_DIR}/time/time.cpp
    ${CMAKE_CURRENT_LIST_DIR}/time/time.h
    ${CMAKE_CURRENT_LIST_DIR}/time/time_posix.cpp
    ${CMAKE_CURRENT_LIST_DIR}/time/time_win.cpp
)

source_group("util" FILES
    ${CMAKE_CURRENT_LIST_DIR}/util/at_exit.cpp
    ${CMAKE_CURRENT_LIST_DIR}/util/at_exit.h
    ${CMAKE_CURRENT_LIST_DIR}/util/base64.cpp
    ${CMAKE_CURRENT_LIST_DIR}/util/base64.h
    ${CMAKE_CURRENT_LIST_DIR}/util/cmd_line_args.cpp
    ${CMAKE_CURRENT_LIST_DIR}/util/cmd_line_args.h
    ${CMAKE_CURRENT_LIST_DIR}/util/stringize_macros.h
    ${CMAKE_CURRENT_LIST_DIR}/util/string_number_conversions.cpp
    ${CMAKE_CURRENT_LIST_DIR}/util/string_number_conversions.h
    ${CMAKE_CURRENT_LIST_DIR}/util/string_util.cpp
    ${CMAKE_CURRENT_LIST_DIR}/util/string_util.h
    ${CMAKE_CURRENT_LIST_DIR}/util/template_util.h
    ${CMAKE_CURRENT_LIST_DIR}/util/unicode.h
    ${CMAKE_CURRENT_LIST_DIR}/util/valuemap.h
)

source_group("win32" FILES
    ${CMAKE_CURRENT_LIST_DIR}/win32/object_watcher.cpp
    ${CMAKE_CURRENT_LIST_DIR}/win32/object_watcher.h
    ${CMAKE_CURRENT_LIST_DIR}/win32/path_util.cpp
    ${CMAKE_CURRENT_LIST_DIR}/win32/path_util.h
    ${CMAKE_CURRENT_LIST_DIR}/win32/platform_string_util.cpp
    ${CMAKE_CURRENT_LIST_DIR}/win32/platform_string_util.h
    ${CMAKE_CURRENT_LIST_DIR}/win32/scoped_win_handle.h
    ${CMAKE_CURRENT_LIST_DIR}/win32/security_util.cpp
    ${CMAKE_CURRENT_LIST_DIR}/win32/security_util.h
    ${CMAKE_CURRENT_LIST_DIR}/win32/shared_memory.cpp
    ${CMAKE_CURRENT_LIST_DIR}/win32/shared_memory.h
    ${CMAKE_CURRENT_LIST_DIR}/win32/win_util.cpp
    ${CMAKE_CURRENT_LIST_DIR}/win32/win_util.h
)

set(SOURCE_FILES
    ${CMAKE_CURRENT_LIST_DIR}/./base.h
    ${CMAKE_CURRENT_LIST_DIR}/./base_config.h
    ${CMAKE_CURRENT_LIST_DIR}/./base_export.h
    ${CMAKE_CURRENT_LIST_DIR}/./base_types.h
    ${CMAKE_CURRENT_LIST_DIR}/./compiler_specific.h
    ${CMAKE_CURRENT_LIST_DIR}/./error.h
    ${CMAKE_CURRENT_LIST_DIR}/./macros.h
    ${CMAKE_CURRENT_LIST_DIR}/./port.h
    ${CMAKE_CURRENT_LIST_DIR}/callback/callback.h
    ${CMAKE_CURRENT_LIST_DIR}/encrypt/des.cpp
    ${CMAKE_CURRENT_LIST_DIR}/encrypt/des.h
    ${CMAKE_CURRENT_LIST_DIR}/encrypt/encrypt.h
    ${CMAKE_CURRENT_LIST_DIR}/encrypt/encrypt_impl.h
    ${CMAKE_CURRENT_LIST_DIR}/file/file_path.h
    ${CMAKE_CURRENT_LIST_DIR}/file/file_util.cpp
    ${CMAKE_CURRENT_LIST_DIR}/file/file_util.h
    ${CMAKE_CURRENT_LIST_DIR}/file/file_util_posix.cpp
    ${CMAKE_CURRENT_LIST_DIR}/file/file_util_win.cpp
    ${CMAKE_CURRENT_LIST_DIR}/framework/default_message_pump.cpp
    ${CMAKE_CURRENT_LIST_DIR}/framework/default_message_pump.h
    ${CMAKE_CURRENT_LIST_DIR}/framework/message_loop.cpp
    ${CMAKE_CURRENT_LIST_DIR}/framework/message_loop.h
    ${CMAKE_CURRENT_LIST_DIR}/framework/message_loop_proxy.cpp
    ${CMAKE_CURRENT_LIST_DIR}/framework/message_loop_proxy.h
    ${CMAKE_CURRENT_LIST_DIR}/framework/message_pump.cpp
    ${CMAKE_CURRENT_LIST_DIR}/framework/message_pump.h
    ${CMAKE_CURRENT_LIST_DIR}/framework/observer_list.h
    ${CMAKE_CURRENT_LIST_DIR}/framework/task.h
    ${CMAKE_CURRENT_LIST_DIR}/framework/win_io_message_pump.cpp
    ${CMAKE_CURRENT_LIST_DIR}/framework/win_io_message_pump.h
    ${CMAKE_CURRENT_LIST_DIR}/framework/win_message_pump.cpp
    ${CMAKE_CURRENT_LIST_DIR}/framework/win_message_pump.h
    ${CMAKE_CURRENT_LIST_DIR}/framework/win_ui_message_pump.cpp
    ${CMAKE_CURRENT_LIST_DIR}/framework/win_ui_message_pump.h
    ${CMAKE_CURRENT_LIST_DIR}/hardware/cpu.cpp
    ${CMAKE_CURRENT_LIST_DIR}/hardware/cpu.h
    ${CMAKE_CURRENT_LIST_DIR}/memory/blockbuffer.h
    ${CMAKE_CURRENT_LIST_DIR}/memory/deleter.h
    ${CMAKE_CURRENT_LIST_DIR}/memory/lazy_instance.h
    ${CMAKE_CURRENT_LIST_DIR}/memory/singleton.h
    ${CMAKE_CURRENT_LIST_DIR}/network/network_util.cpp
    ${CMAKE_CURRENT_LIST_DIR}/network/network_util.h
    ${CMAKE_CURRENT_LIST_DIR}/network/nio_base.h
    ${CMAKE_CURRENT_LIST_DIR}/synchronization/lock.h
    ${CMAKE_CURRENT_LIST_DIR}/synchronization/lock_posix.cpp
    ${CMAKE_CURRENT_LIST_DIR}/synchronization/lock_win.cpp
    ${CMAKE_CURRENT_LIST_DIR}/synchronization/semaphore.h
    ${CMAKE_CURRENT_LIST_DIR}/synchronization/semaphore_posix.cpp
    ${CMAKE_CURRENT_LIST_DIR}/synchronization/semaphore_win.cpp
    ${CMAKE_CURRENT_LIST_DIR}/synchronization/waitable_event.h
    ${CMAKE_CURRENT_LIST_DIR}/synchronization/waitable_event_posix.cpp
    ${CMAKE_CURRENT_LIST_DIR}/synchronization/waitable_event_win.cpp
    ${CMAKE_CURRENT_LIST_DIR}/third_party/chrome/atomicops.h
    ${CMAKE_CURRENT_LIST_DIR}/third_party/chrome/atomicops_internals_arm_gcc.h
    ${CMAKE_CURRENT_LIST_DIR}/third_party/chrome/atomicops_internals_atomicword_compat.h
    ${CMAKE_CURRENT_LIST_DIR}/third_party/chrome/atomicops_internals_x86_gcc.cc
    ${CMAKE_CURRENT_LIST_DIR}/third_party/chrome/atomicops_internals_x86_gcc.h
    ${CMAKE_CURRENT_LIST_DIR}/third_party/chrome/atomicops_internals_x86_macosx.h
    ${CMAKE_CURRENT_LIST_DIR}/third_party/chrome/atomicops_internals_x86_msvc.h
    ${CMAKE_CURRENT_LIST_DIR}/third_party/convert_utf/ConvertUTF.c
    ${CMAKE_CURRENT_LIST_DIR}/third_party/convert_utf/ConvertUTF.h
    ${CMAKE_CURRENT_LIST_DIR}/third_party/modp_b64/modp_b64.cc
    ${CMAKE_CURRENT_LIST_DIR}/third_party/modp_b64/modp_b64.h
    ${CMAKE_CURRENT_LIST_DIR}/third_party/modp_b64/modp_b64_data.h
    ${CMAKE_CURRENT_LIST_DIR}/thread/framework_thread.cpp
    ${CMAKE_CURRENT_LIST_DIR}/thread/framework_thread.h
    ${CMAKE_CURRENT_LIST_DIR}/thread/thread.h
    ${CMAKE_CURRENT_LIST_DIR}/thread/thread_local.h
    ${CMAKE_CURRENT_LIST_DIR}/thread/thread_local_posix.cpp
    ${CMAKE_CURRENT_LIST_DIR}/thread/thread_local_win.cpp
    ${CMAKE_CURRENT_LIST_DIR}/thread/thread_manager.cpp
    ${CMAKE_CURRENT_LIST_DIR}/thread/thread_manager.h
    ${CMAKE_CURRENT_LIST_DIR}/thread/thread_posix.cpp
    ${CMAKE_CURRENT_LIST_DIR}/thread/thread_win.cpp
    ${CMAKE_CURRENT_LIST_DIR}/time/time.cpp
    ${CMAKE_CURRENT_LIST_DIR}/time/time.h
    ${CMAKE_CURRENT_LIST_DIR}/time/time_posix.cpp
    ${CMAKE_CURRENT_LIST_DIR}/time/time_win.cpp
    ${CMAKE_CURRENT_LIST_DIR}/util/at_exit.cpp
    ${CMAKE_CURRENT_LIST_DIR}/util/at_exit.h
    ${CMAKE_CURRENT_LIST_DIR}/util/base64.cpp
    ${CMAKE_CURRENT_LIST_DIR}/util/base64.h
    ${CMAKE_CURRENT_LIST_DIR}/util/cmd_line_args.cpp
    ${CMAKE_CURRENT_LIST_DIR}/util/cmd_line_args.h
    ${CMAKE_CURRENT_LIST_DIR}/util/stringize_macros.h
    ${CMAKE_CURRENT_LIST_DIR}/util/string_number_conversions.cpp
    ${CMAKE_CURRENT_LIST_DIR}/util/string_number_conversions.h
    ${CMAKE_CURRENT_LIST_DIR}/util/string_util.cpp
    ${CMAKE_CURRENT_LIST_DIR}/util/string_util.h
    ${CMAKE_CURRENT_LIST_DIR}/util/template_util.h
    ${CMAKE_CURRENT_LIST_DIR}/util/unicode.h
    ${CMAKE_CURRENT_LIST_DIR}/util/valuemap.h
    ${CMAKE_CURRENT_LIST_DIR}/win32/object_watcher.cpp
    ${CMAKE_CURRENT_LIST_DIR}/win32/object_watcher.h
    ${CMAKE_CURRENT_LIST_DIR}/win32/path_util.cpp
    ${CMAKE_CURRENT_LIST_DIR}/win32/path_util.h
    ${CMAKE_CURRENT_LIST_DIR}/win32/platform_string_util.cpp
    ${CMAKE_CURRENT_LIST_DIR}/win32/platform_string_util.h
    ${CMAKE_CURRENT_LIST_DIR}/win32/scoped_win_handle.h
    ${CMAKE_CURRENT_LIST_DIR}/win32/security_util.cpp
    ${CMAKE_CURRENT_LIST_DIR}/win32/security_util.h
    ${CMAKE_CURRENT_LIST_DIR}/win32/shared_memory.cpp
    ${CMAKE_CURRENT_LIST_DIR}/win32/shared_memory.h
    ${CMAKE_CURRENT_LIST_DIR}/win32/win_util.cpp
    ${CMAKE_CURRENT_LIST_DIR}/win32/win_util.h
)

add_library(${PROJECT_NAME} ${SOURCE_FILES})
