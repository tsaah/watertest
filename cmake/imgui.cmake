add_library(imgui STATIC)

target_sources(imgui PRIVATE
    ${EXTERNAL_DIR}/imgui/imgui_demo.cpp
    ${EXTERNAL_DIR}/imgui/imgui_draw.cpp
    ${EXTERNAL_DIR}/imgui/imgui_tables.cpp
    ${EXTERNAL_DIR}/imgui/imgui_widgets.cpp
    ${EXTERNAL_DIR}/imgui/imgui.cpp
    ${EXTERNAL_DIR}/imgui/backends/imgui_impl_opengl3.cpp
    ${EXTERNAL_DIR}/imgui/backends/imgui_impl_glfw.cpp
)

target_include_directories(imgui
    PUBLIC ${EXTERNAL_DIR}/imgui
    PUBLIC ${EXTERNAL_DIR}/imgui/backends
    PRIVATE ${EXTERNAL_DIR}/glfw/include
)
