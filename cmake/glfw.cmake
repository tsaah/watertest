add_definitions(
    -D_GLFW_WIN32
)

add_library(glfw STATIC
    ${EXTERNAL_DIR}/glfw/src/context.c
    ${EXTERNAL_DIR}/glfw/src/egl_context.c
    ${EXTERNAL_DIR}/glfw/src/init.c
    ${EXTERNAL_DIR}/glfw/src/input.c
    ${EXTERNAL_DIR}/glfw/src/monitor.c
    ${EXTERNAL_DIR}/glfw/src/null_init.c
    ${EXTERNAL_DIR}/glfw/src/null_joystick.c
    ${EXTERNAL_DIR}/glfw/src/null_monitor.c
    ${EXTERNAL_DIR}/glfw/src/null_window.c
    ${EXTERNAL_DIR}/glfw/src/osmesa_context.c
    ${EXTERNAL_DIR}/glfw/src/platform.c
    ${EXTERNAL_DIR}/glfw/src/vulkan.c
    ${EXTERNAL_DIR}/glfw/src/wgl_context.c
    ${EXTERNAL_DIR}/glfw/src/win32_init.c
    ${EXTERNAL_DIR}/glfw/src/win32_joystick.c
    ${EXTERNAL_DIR}/glfw/src/win32_module.c
    ${EXTERNAL_DIR}/glfw/src/win32_monitor.c
    ${EXTERNAL_DIR}/glfw/src/win32_thread.c
    ${EXTERNAL_DIR}/glfw/src/win32_time.c
    ${EXTERNAL_DIR}/glfw/src/win32_window.c
    ${EXTERNAL_DIR}/glfw/src/window.c
)

target_include_directories(glfw PUBLIC
    ${EXTERNAL_DIR}/glfw/include
)
