/*****************************************************************************/
/*  Copyright (c) 2016, Alessandro Pieropan                                  */
/*  All rights reserved.                                                     */
/*                                                                           */
/*  Redistribution and use in source and binary forms, with or without       */
/*  modification, are permitted provided that the following conditions       */
/*  are met:                                                                 */
/*                                                                           */
/*  1. Redistributions of source code must retain the above copyright        */
/*  notice, this list of conditions and the following disclaimer.            */
/*                                                                           */
/*  2. Redistributions in binary form must reproduce the above copyright     */
/*  notice, this list of conditions and the following disclaimer in the      */
/*  documentation and/or other materials provided with the distribution.     */
/*                                                                           */
/*  3. Neither the name of the copyright holder nor the names of its         */
/*  contributors may be used to endorse or promote products derived from     */
/*  this software without specific prior written permission.                 */
/*                                                                           */
/*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS      */
/*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT        */
/*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR    */
/*  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT     */
/*  HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,   */
/*  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT         */
/*  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,    */
/*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY    */
/*  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT      */
/*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE    */
/*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.     */
/*****************************************************************************/

// Std. Includes
#include <string>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include "shader.h"
#include "camera.h"
#include "model.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other Libs
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

//#include <SOIL.h>
#include <iostream>


// Properties
GLuint screenWidth = 800, screenHeight = 600;

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action,
                  int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void Do_Movement();

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

// The MAIN function, from here we start our application and run our Game loop
int main() {
  // Init GLFW
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  GLFWwindow* window = glfwCreateWindow(
      screenWidth, screenHeight, "LearnOpenGL", nullptr, nullptr);  // Windowed
  glfwMakeContextCurrent(window);

  // Set the required callback functions
  glfwSetKeyCallback(window, key_callback);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetScrollCallback(window, scroll_callback);

  // Options
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  // Initialize GLEW to setup the OpenGL Function pointers
  glewExperimental = GL_TRUE;
  glewInit();

  // Define the viewport dimensions
  glViewport(0, 0, screenWidth, screenHeight);

  // Setup some OpenGL options
  glEnable(GL_DEPTH_TEST);

  // Setup and compile our shaders
  rendering::Shader shader(
      "/home/alessandro/projects/rendering_engine/data/shaders/simple.vs",
      "/home/alessandro/projects/rendering_engine/data/shaders/simple.frag");

  // Load models
  // Model
  // ourModel("/home/alessandro/projects/drone_ws/src/fato_tracker/data/ros_hydro/ros_hydro.obj");

  // Draw in wireframe
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  GLfloat vertices[] = {
      // Positions          // Colors           // Texture Coords
      0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
      0.0f,  1.0f,  1.0f,  // Top Right
      0.5f,  -0.5f, 0.0f, 0.0f, 1.0f,
      0.0f,  1.0f,  0.0f,  // Bottom Right
      -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
      1.0f,  0.0f,  0.0f,  // Bottom Left
      -0.5f, 0.5f,  0.0f, 1.0f, 1.0f,
      0.0f,  0.0f,  1.0f  // Top Left
  };
  GLuint indices[] = {  // Note that we start from 0!
          0, 1, 3, // First Triangle
          1, 2, 3  // Second Triangle
      };

  GLuint VBO, VAO, EBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);
  // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and
  // attribute pointer(s).
  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
                        (GLvoid*)0);
  glEnableVertexAttribArray(0);
  // Color attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
                        (GLvoid*)(3 * sizeof(GLfloat)));
  glEnableVertexAttribArray(1);

  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
                        (GLvoid*)(6 * sizeof(GLfloat)));
  glEnableVertexAttribArray(2);


  glBindVertexArray(0);  // Unbind VAO (it's always a good thing to unbind any
                         // buffer/array to prevent strange bugs)

  // Load and create a texture
  GLuint texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);  // All upcoming GL_TEXTURE_2D
                                          // operations now have effect on this
                                          // texture object
  // Set the texture wrapping parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                  GL_REPEAT);  // Set texture wrapping to GL_REPEAT (usually
                               // basic wrapping method)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  // Set texture filtering parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // Load image, create texture and generate mipmaps

  cv::Mat rgb_img = cv::imread("/home/alessandro/projects/rendering_engine/data/textures/container.jpg");
  cv::cvtColor(rgb_img, rgb_img, CV_BGR2RGB);
  //unsigned char* image = SOIL_load_image(
  //    "/home/alessandro/projects/rendering_engine/data/textures/container.jpg",
  //    &width, &height, 0, SOIL_LOAD_RGB);
  unsigned char* img_data = rgb_img.data;


  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, rgb_img.cols, rgb_img.rows, 0, GL_RGB,
               GL_UNSIGNED_BYTE, img_data);
  glGenerateMipmap(GL_TEXTURE_2D);
  //SOIL_free_image_data(image);
  glBindTexture(GL_TEXTURE_2D, 0);  // Unbind texture when done, so we won't


  // accidentily mess up our texture.

  // Game loop
  while (!glfwWindowShouldClose(window)) {
    // Set frame time
    GLfloat currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    // Check and call events
    glfwPollEvents();
    // Do_Movement();

    // Clear the colorbuffer
    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Bind Texture
    glBindTexture(GL_TEXTURE_2D, texture);

    shader.use();  // <-- Don't forget this one!

    // Draw our first triangle
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // Swap the buffers
    glfwSwapBuffers(window);
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
  // Terminate GLFW, clearing any resources allocated by GLFW.
  glfwTerminate();
  return 0;
}

// Moves/alters the camera positions based on user input
void Do_Movement() {
  // Camera controls
  if (keys[GLFW_KEY_W]) camera.ProcessKeyboard(FORWARD, deltaTime);
  if (keys[GLFW_KEY_S]) camera.ProcessKeyboard(BACKWARD, deltaTime);
  if (keys[GLFW_KEY_A]) camera.ProcessKeyboard(LEFT, deltaTime);
  if (keys[GLFW_KEY_D]) camera.ProcessKeyboard(RIGHT, deltaTime);
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action,
                  int mode) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);

  if (action == GLFW_PRESS)
    keys[key] = true;
  else if (action == GLFW_RELEASE)
    keys[key] = false;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
  if (firstMouse) {
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;
  }

  GLfloat xoffset = xpos - lastX;
  GLfloat yoffset = lastY - ypos;

  lastX = xpos;
  lastY = ypos;

  camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
  camera.ProcessMouseScroll(yoffset);
}
