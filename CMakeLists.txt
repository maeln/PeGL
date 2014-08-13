cmake_minimum_required(VERSION 2.8)

project(PeGL)

find_package(PkgConfig REQUIRED)

# finds OpenGL, GLU and X11
find_package(OpenGL REQUIRED)
if(NOT OPENGL_FOUND)
    message("ERROR: OpenGL not found")
endif(NOT OPENGL_FOUND)
set(GL_LIBRARY GL GLU GLEW X11)

# GLFW
pkg_search_module(GLFW REQUIRED glfw3)
include_directories(${GLFW_INCLUDE_DIRS})

add_executable(
	../PeGL 
	PeGL.cxx 
	shaderloader.cxx 
	imageloader.cxx 
	meshloader.cxx 
	objectmanager.cxx 
	matrixstack.cxx 
	postprocessing.cxx 
	camera.cxx 
	mainloop.cxx
	pewindow.cxx
)

target_link_libraries(
	../PeGL 
	${GL_LIBRARY} 
	${GLFW_STATIC_LIBRARIES}
	SOIL
	m
)
