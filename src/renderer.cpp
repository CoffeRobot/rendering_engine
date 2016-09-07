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
#include "renderer.h"

#include <stdexcept>

using namespace std;

namespace rendering {

Renderer::Renderer(int image_width, int image_height, float fx, float fy,
                   float cx, float cy, float near_plane, float far_plane)
    : width_(image_width),
      height_(image_height),
      fx_(fx),
      fy_(fy),
      cx_(cx),
      cy_(cy),
      near_plane_(near_plane),
      far_plane_(far_plane),
      camera_(glm::vec3(0.0, 0.0, 0.0),glm::vec3(0.0, 1.0, 0.0),glm::vec3(0.0, 0.0, -1.0))
{
    updateProjectionMatrix(fx,fy,cx,cy,near_plane,far_plane);

//    glm::mat4 view = camera_.GetViewMatrix();
//    cout << "renderer camera matrix " << endl;
//    for (int i = 0; i < 4; ++i) {
//      for (int j = 0; j < 4; ++j) {
//        cout << view[i][j] << " ";
//      }
//      cout << "\n";
//    }
}

Renderer::~Renderer() { glfwTerminate(); }

void Renderer::initRenderContext(int width, int height) {
  //cout << "getting here" << endl;

  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  window_ = glfwCreateWindow(width, height, "TrackerRenderer", nullptr,
                             nullptr);  // Windowed
  glfwMakeContextCurrent(window_);

  glewExperimental = GL_TRUE;
  glewInit();

  // Define the viewport dimensions
  glViewport(0, 0, width, height);

  glEnable(GL_DEPTH_TEST);

  //cout << "ending init" << endl;
}

void Renderer::updateProjectionMatrix(float fx, float fy, float cx, float cy,
                                      float near_plane, float far_plane) {
  projection_matrix_ = glm::mat4(0.0);

  fx_ = fx;
  fy_ = fy;
  cx_ = cx;
  cy_ = cy;
  near_plane_ = near_plane;
  far_plane_ = far_plane;
  float zoom_x = 1;
  float zoom_y = 1;

  // on the contrary of OGRE, projection matrix should be row major!
  projection_matrix_[0][0] = 2.0 * fx / (float)width_ * zoom_x;
  projection_matrix_[1][1] = 2.0 * fy / (float)height_ * zoom_y;
  projection_matrix_[2][0] = 2.0 * (0.5 - cx / (float)width_) * zoom_x;
  projection_matrix_[2][1] = 2.0 * (cy / (float)height_ - 0.5) * zoom_y;
  projection_matrix_[2][2] =
      -(far_plane + near_plane) / (far_plane - near_plane);
  projection_matrix_[3][2] =
      -2.0 * far_plane * near_plane / (far_plane - near_plane);
  projection_matrix_[2][3] = -1;
}

void Renderer::updateProjectionMatrix(glm::mat4 projection)
{
    projection_matrix_ = projection;
}

void Renderer::updateCamera(glm::vec3 position, glm::vec3 orientation,
                            glm::mat4 projection_matrix) {}

void Renderer::render() {
  // Check and call events
  glfwPollEvents();

  // Clear the colorbuffer
  glClearColor(0.00f, 0.00f, 0.00f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  for (RigidObject& obj : objects_) {
    if (!obj.isVisible()) continue;

    Shader& shader = obj.shader;
    // activating shader
    shader.use();

    glm::mat4 view = camera_.GetViewMatrix();
    // sending to shader custom perspective projection matrix
    glUniformMatrix4fv(glGetUniformLocation(shader.program_id_, "projection"),
                       1, GL_FALSE, glm::value_ptr(projection_matrix_));
    // sending to shader camera view matris
    glUniformMatrix4fv(glGetUniformLocation(shader.program_id_, "view"), 1,
                       GL_FALSE, glm::value_ptr(view));
    // sending to shader model matrix
    glUniformMatrix4fv(glGetUniformLocation(shader.program_id_, "model"), 1,
                       GL_FALSE, glm::value_ptr(obj.model_matrix_));
    // drawing the model with the current shader
    obj.model.draw(shader);
  }

  glfwSwapBuffers(window_);
}

void Renderer::addModel(string model_name, string ver_shader,
                        string frag_shader) {
  RigidObject obj(model_name, ver_shader, frag_shader);
  objects_.push_back(obj);
}

RigidObject& Renderer::getObject(int id) {
  if (id < objects_.size())
    return objects_[id];
  else
    throw runtime_error("Renderer: object id out of bound!");
}
}
