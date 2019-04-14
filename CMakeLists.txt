cmake_minimum_required(VERSION 2.8)
project(GLAB)

set(CMAKE_CXX_STANDARD 11)
include_directories(glm)
add_executable(GLAB main.cpp point.h Shader.cpp Shader.h Attrib.cpp Attrib.h VBO.cpp VBO.h Material.cpp Material.h Uniforms.cpp Uniforms.h stb_image.h stb_image.cpp Texture2D.cpp Texture2D.h EBO.h)
target_link_libraries(GLAB GLEW glut GL)
