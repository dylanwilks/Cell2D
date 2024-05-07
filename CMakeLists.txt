cmake_minimum_required(VERSION 3.0...3.20 FATAL_ERROR)

project(cell_project)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic \
                    -lglfw3 -lGL -lX11 -lpthread -lXrandr -lXi -ldl \
                    -O0 -g")
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR})

find_package(glfw3 3.3 REQUIRED)
find_package(OpenGL REQUIRED)
message(STATUS "Found GLFW3 in ${GLFW3_INCLUDE_DIR}")


option(NOGRAPHICS "Build without graphics" OFF)
#add feature to control flags for method of collision handling in the future

#include(include/CMakeLists.txt)
include(src/CMakeLists.txt)

if(NOGRAPHICS)
  #build without graphics

  add_executable(cells_nographics.elf ${c2_sources} src/testing.cpp)
  target_include_directories(cells_nographics.elf PUBLIC "${CMAKE_SOURCE_DIR}/include/")
else()
  #build with graphics (default)
  configure_file(graphics/src/gui/bitmapfont.png COPYONLY)

  #add_subdirectory(graphics)
  include(graphics/src/CMakeLists.txt)
  add_executable(cells.elf ${c2_graphics} ${c2_sources} graphics/src/example.cpp)
  target_include_directories(cells.elf PUBLIC "${CMAKE_SOURCE_DIR}/graphics/include/" 
                                                 "${CMAKE_SOURCE_DIR}/include/" 
                                                  ${GLFW3_INCLUDE_DIR})
  target_link_libraries(cells.elf glfw GL ${CMAKE_DL_LIBS} ${GLFW3_LIBRARY} ${OPENGL_gl_LIBRARY})
endif()
