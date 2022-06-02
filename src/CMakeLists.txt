file(GLOB_RECURSE SOURCES *.h *.cpp)
file(GLOB_RECURSE SHADERS *.frag *.vert *.geom *.tesc *.tese)
file(GLOB_RECURSE TEXTURES *.jpg *.png *.tif *.tiff)



add_executable(app
    ${SOURCES}
)

target_link_libraries(app PRIVATE
    glfw
    imgui
    glad
    glm
    stb
)

install_targets(bin app)
install_files(bin/shaders FILES ${SHADERS})
install_files(bin/textures FILES ${TEXTURES})