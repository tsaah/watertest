add_library(glm INTERFACE)

target_include_directories(glm PUBLIC INTERFACE
    ${EXTERNAL_DIR}/glm
)