cmake_minimum_required(VERSION 2.8.7)
project(fato_rendering)


find_package(OpenCV REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)


# set this path to where OGRE is located
set(CMAKE_MODULE_PATH "/home/alessandro/ogre/lib/OGRE/cmake/;${CMAKE_MODULE_PATH} ")
# custom findASSIMP defined in the local folder
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules/;${CMAKE_MODULE_PATH} ")

find_package(GLFW REQUIRED)
find_package(ASSIMP REQUIRED)
find_package(Eigen REQUIRED)
add_definitions(${Eigen_DEFINITIONS})


SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# show additional files in qtcreator
execute_process(COMMAND find ${PROJECT_SOURCE_DIR} -type f
  OUTPUT_VARIABLE FILES_STRING)
string(REPLACE "\n" ";" FILES_LIST ${FILES_STRING})
add_custom_target(additional_files_${PROJECT_NAME} SOURCES
  ${FILES_LIST}
  ${PROJECT_SOURCE_DIR}/README.md
)

include_directories(include
  ${Eigen_INCLUDE_DIR}
  ${OpenCV_INCLUDE_DIRS}
  ${urdfdom_INCLUDE_DIRS}
  ${CUDA_INCLUDE_DIRS}
  ${GLFW_INCLUDE_DIR}
)

add_executable(${PROJECT_NAME}
  src/opengl_integration.cpp
  src/shader.cpp
  src/camera.cpp
  src/mesh.cpp
  src/model.cpp
  src/renderer.cpp
  src/rigid_object.cpp
 )

target_link_libraries(${PROJECT_NAME}
 ${OPENCV_LIBS}
 ${Boost_LIBRARIES}
 ${OpenCV_LIBS}
 ${GLEW_LIBRARIES}
 ${OPENGL_LIBRARIES}
 ${GLFW_LIBRARIES}
 ${assimp_LIBRARIES}
 )

