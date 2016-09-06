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

#include "rigid_object.h"

namespace rendering {

RigidObject::RigidObject(std::string model, std::string ver_shader,
                         std::string frag_shader)
    : model((GLchar*)model.c_str()),
      shader((GLchar*)ver_shader.c_str(), (GLchar*)frag_shader.c_str()),
model_matrix_(),
is_visible_(true)
{

}

RigidObject::~RigidObject() {}

void RigidObject::setVisible(bool is_visible) { is_visible_ = is_visible; }

bool RigidObject::isVisible() { return is_visible_; }

void RigidObject::updatePose(Eigen::Transform<double, 3, Eigen::Affine>& pose) {
//  for(int i = 0; i < 3; ++i)
//  {
//      for(int j = 0; j < 3; ++j)
//      {
//          model_matrix_[i][j] = pose(i,j);
//      }
//      model_matrix_[i][3] = pose(i,3);
//  }
//  model_matrix_[3][3] = 1;



    model_matrix_ = glm::translate(
        model_matrix_,
        glm::vec3(0.0f, 0.0f, -1.0f));
}

}  // end namespace
