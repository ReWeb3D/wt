/*
	This file is part of ReWeb3D.

    ReWeb3D is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License.

    ReWeb3D is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with ReWeb3D.  If not, see <http://www.gnu.org/licenses/>.

    Copyright (C) 2011 Vicomtech Foundation, 
	Donostia - San Sebastian, Spain.
    All Rights Reserved.

    Written by Mauricio Aristizabal <maristi7@eafit.edu.co>
*/


/*
 * Based on lesson 1 of webgl-lessons http://learningwebgl.com/blog/?p=28
 */

#include <Wt/WApplication>
#include <Wt/WBreak>
#include <Wt/WContainerWidget>
#include <Wt/WImage>
#include <Wt/WPushButton>
#include <Wt/WTabWidget>
#include <Wt/WText>
#include <Wt/WTextArea>




#include "DrawGLWidget.h"

using namespace Wt;

const std::string fragShad("#ifdef GL_ES \n"
                           "precision highp float; \n"
                           "#endif \n"
                           " \n"
                           "void main(void) { \n"
                           "    gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0); \n"
                           "} \n");
const std::string vertShad("attribute vec3 aVertexPosition; \n"
                           " \n"
                           "uniform mat4 uMVMatrix; \n"
                           "uniform mat4 uPMatrix; \n"
                           " \n"
                           "void main(void) { \n"
                           "    gl_Position = uPMatrix * uMVMatrix * vec4(aVertexPosition, 1.0);\n"
                           "} \n");

/*
 * A pretty basic WebGL demo application
 */
class WebGLDemo : public WApplication
{
public:
  WebGLDemo(const WEnvironment& env);

private:
  void updateShaders();
  void resetShaders();

  WContainerWidget *glContainer_;
  DrawGLWidget *drawGLWidget_;
  WTextArea *fragmentShaderText_;
  WTextArea *vertexShaderText_;
};

WebGLDemo::WebGLDemo(const WEnvironment& env)
  : WApplication(env)
{
  setTitle("Demo");

  root()->addWidget(new WText("Wt/WebGL Lesson 1: A Triangle and a Square"));
  root()->addWidget(new WBreak());

  drawGLWidget_ = 0;

  glContainer_ = new WContainerWidget(root());
  glContainer_->resize(500, 500);
  glContainer_->setInline(false);

  updateShaders();

  /////////////////////////////////////////////
  root()->addWidget(new WBreak());
  root()->addWidget(new WBreak());
  root()->addWidget(new WBreak());
  root()->addWidget(new WText("This demo has been developed by Vicomtech-IK4 Research Center"));

  root()->addWidget(new WBreak());
  root()->addWidget(new WText("Based on the learningwebgl.com lessons."));
  root()->addWidget(new WBreak());
  root()->addWidget(new WText("www.vicomtech.es"));

  //root()->addWidget(new WBreak());
  //root()->addWidget(new WImage("vicomLogoSm.png"));
  /////////////////////////////////////////////

}

void WebGLDemo::updateShaders()
{
  delete drawGLWidget_;
  drawGLWidget_ = new DrawGLWidget(glContainer_);
  drawGLWidget_->resize(500, 500);
  drawGLWidget_->setShaders(vertShad,fragShad);
  drawGLWidget_->setAlternativeContent(new WImage("nowebgl.png"));
}

void WebGLDemo::resetShaders()
{
  updateShaders();
}

WApplication *createApplication(const WEnvironment& env)
{
  return new WebGLDemo(env);
}

int main(int argc, char **argv)
{
  return WRun(argc, argv, &createApplication);
}

