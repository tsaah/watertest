add_library(glad STATIC
    ${EXTERNAL_DIR}/glad/src/glad.c
)

target_include_directories(glad PUBLIC
    ${EXTERNAL_DIR}/glad/include
)