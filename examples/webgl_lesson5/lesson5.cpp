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
    along with ReWeb3D. If not, see <http://www.gnu.org/licenses/>.

    Copyright (C) 2011 Vicomtech Foundation, 
	Donostia - San Sebastian, Spain.
    All Rights Reserved.

    Written by Mauricio Aristizabal <maristi7@eafit.edu.co>
*/

/*
 * Lesson 2 of webgl-lessons http://learningwebgl.com/blog/?p=28
 */

#include <Wt/WApplication>
#include <Wt/WBreak>
#include <Wt/WContainerWidget>
#include <Wt/WImage>
#include <Wt/WText>
#include <Wt/WTimer>
//#include <Wt/WPushButton>
#include "DrawGLWidget.h"


using namespace Wt;

const std::string fragShad(
"#ifdef GL_ES \n"
"precision highp float; \n"
"#endif \n"
" \n"
"varying vec2 vTextureCoord; \n"
" \n"
"uniform sampler2D uSampler; \n"
" \n"
"void main(void) { \n"
"    gl_FragColor = texture2D(uSampler, vec2(vTextureCoord.s, vTextureCoord.t));//vec4(vTextureCoord.x,vTextureCoord.y,1.0,1.0) ;// \n"
"} \n");


const std::string vertShad(
"attribute vec3 aVertexPosition; \n"
"attribute vec2 aTextureCoord; \n"
" \n"
"uniform mat4 uMVMatrix; \n"
"uniform mat4 uPMatrix; \n"
"uniform mat4 uCMatrix; \n"
" \n"
"varying vec2 vTextureCoord; \n"
" \n"
" \n"
"void main(void) { \n"
"    gl_Position = uPMatrix * uMVMatrix * uCMatrix * vec4(aVertexPosition, 1.0); \n"
"    vTextureCoord = aTextureCoord; \n"
"} \n");



/*
 * A pretty basic WebGL demo application
 */
class WebGLDemo : public WApplication
{
public:
  WebGLDemo(const WEnvironment& env);
  void animate();

private:
  void updateShaders();
  void resetShaders();

  WContainerWidget *glContainer_;
  DrawGLWidget *drawGLWidget_;
  WTimer *timer;
  WImage *wario;

};





WebGLDemo::WebGLDemo(const WEnvironment& env)
  : WApplication(env)
{
  setTitle("Demo");

  root()->addWidget(new WText("Wt/WebGL Lesson 5: Introducing Textures"));
  root()->addWidget(new WBreak());

  drawGLWidget_ = 0;

  glContainer_ = new WContainerWidget(root());
  glContainer_->resize(500, 500);
  glContainer_->setInline(false);
  root()->addWidget(new WBreak());
  //wario=new WImage("http://192.168.65.15:8080/thredds/wms?dataset=http://www.esrl.noaa.gov/psd/thredds/dodsC/Datasets/godas/ucur.2010.nc&LAYERS=ucur&ELEVATION=-5&TIME=2010-12-01T00:00:00.000Z&TRANSPARENT=false&STYLES=boxfill/greyscale&CRS=EPSG:4326&COLORSCALERANGE=-3,3&NUMCOLORBANDS=200&LOGSCALE=false&SERVICE=WMS&VERSION=1.3.0&REQUEST=GetMap&EXCEPTIONS=XML&FORMAT=image/png&BBOX=-180,-90,180,90&WIDTH=512&HEIGHT=512",root());
  updateShaders();

  //Set up the Timer in order to animate the scene.
  timer = new WTimer(drawGLWidget_);
  timer->setInterval(25);
  timer->start();
  timer->timeout().connect(this,&WebGLDemo::animate);// */

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
    //delete drawGLWidget_;
    drawGLWidget_ = new DrawGLWidget(glContainer_,wario);
    drawGLWidget_->resize(500, 500);
    drawGLWidget_->setShaders(vertShad,fragShad);
    drawGLWidget_->setAlternativeContent(new WImage("nowebgl.png"));

}
void WebGLDemo::animate()
{
    // Call the repaintGL() to update the paint event
    drawGLWidget_->repaintGL(DrawGLWidget::PAINT_GL);
}

WApplication *createApplication(const WEnvironment& env)
{
  return new WebGLDemo(env);
}

int main(int argc, char **argv)
{
  return WRun(argc, argv, &createApplication);
}

