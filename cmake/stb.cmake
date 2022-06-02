add_definitions(-DSTB_IMAGE_IMPLEMENTATION)

add_library(stb INTERFACE)

target_include_directories(stb PUBLIC INTERFACE
    ${EXTERNAL_DIR}/stb
)