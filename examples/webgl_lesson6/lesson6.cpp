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
#include <Wt/WEvent>
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
  void eventKeyWentDown(const WKeyEvent& event);

private:
  void updateShaders();
  void resetShaders();

  WContainerWidget *glContainer_;
  DrawGLWidget *drawGLWidget_;
  WTimer *timer;
  WEvent *event;
  WImage *wario;
};



void WebGLDemo::eventKeyWentDown(const WKeyEvent& event)
{
    Key e = event.key();

    if (e == Key_F ) {// change if "f" is pressed.
        drawGLWidget_->changeTextureFilters();
        drawGLWidget_->repaintGL(DrawGLWidget::PAINT_GL);
    }else if (e ==  Key_PageDown ) {// change if "avpag" is pressed.
        drawGLWidget_->decrementZDepth();
        drawGLWidget_->repaintGL(DrawGLWidget::PAINT_GL);
    }else if (e ==  Key_PageUp ) {// change if "repag" is pressed.
        drawGLWidget_->incrementZDepth();
        drawGLWidget_->repaintGL(DrawGLWidget::PAINT_GL);
    }else if (e ==  Key_Up ) {// change if "up" is pressed.
        drawGLWidget_->incrementRotX();
        drawGLWidget_->repaintGL(DrawGLWidget::PAINT_GL);
    }else if (e ==  Key_Down ) {// change if "down" is pressed.
        drawGLWidget_->decrementRotX();
        drawGLWidget_->repaintGL(DrawGLWidget::PAINT_GL);
    }else if (e ==  Key_Right ) {// change if "right" is pressed.
        drawGLWidget_->incrementRotY();
        drawGLWidget_->repaintGL(DrawGLWidget::PAINT_GL);
    }else if (e ==  Key_Left ) {// change if "left" is pressed.
        drawGLWidget_->decrementRotY();
        drawGLWidget_->repaintGL(DrawGLWidget::PAINT_GL);
    }





}

WebGLDemo::WebGLDemo(const WEnvironment& env)
  : WApplication(env)
{
    std::string googAnalytics(" var _gaq = _gaq || [];"
                              "_gaq.push(['_setAccount', 'UA-22447986-1']);"
                              "_gaq.push(['_trackPageview']);"
                              ""
                              "(function() {"
                              "  var ga = document.createElement('script'); ga.type = 'text/javascript'; ga.async = true;"
                              "  ga.src = ('https:' == document.location.protocol ? 'https://ssl' : 'http://www') + '.google-analytics.com/ga.js';"
                              "  var s = document.getElementsByTagName('script')[0]; s.parentNode.insertBefore(ga, s);"
                              "})();");

   doJavaScript(googAnalytics);
  setTitle("Demo");

  root()->addWidget(new WText("Wt/WebGL Lesson 6: Keyboard Input and Texture Filters"));
  root()->addWidget(new WBreak());
  drawGLWidget_ = 0;

  glContainer_ = new WContainerWidget(root());
  glContainer_->resize(500, 500);
  glContainer_->setInline(false);
  //wario=new WImage("http://192.168.65.15:8080/thredds/wms?dataset=http://www.esrl.noaa.gov/psd/thredds/dodsC/Datasets/godas/ucur.2010.nc&LAYERS=ucur&ELEVATION=-5&TIME=2010-12-01T00:00:00.000Z&TRANSPARENT=false&STYLES=boxfill/greyscale&CRS=EPSG:4326&COLORSCALERANGE=-3,3&NUMCOLORBANDS=200&LOGSCALE=false&SERVICE=WMS&VERSION=1.3.0&REQUEST=GetMap&EXCEPTIONS=XML&FORMAT=image/png&BBOX=-180,-90,180,90&WIDTH=512&HEIGHT=512",root());
  root()->addWidget(new WBreak());
  root()->addWidget(new WText("Rotate with up/down/left/right keys and zoom with page up/page down"));


  updateShaders();


  //root()->keyWentDown().connect(this,&WebGLDemo::eventKeyWentDown);
  this->globalKeyWentDown().connect(this,&WebGLDemo::eventKeyWentDown);



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
    drawGLWidget_ = new DrawGLWidget(glContainer_);
    drawGLWidget_->resize(500, 500);
    drawGLWidget_->setShaders(vertShad,fragShad);
    drawGLWidget_->setAlternativeContent(new WImage("nowebgl.png"));

}
void WebGLDemo::animate()
{
    // Call the repaintGL() to update the paint event
    drawGLWidget_->updateMotion();
    drawGLWidget_->repaintGL(DrawGLWidget::PAINT_GL);
    // drawGLWidget_->repaintSlot(); // For client-side replots
}

WApplication *createApplication(const WEnvironment& env)
{
  return new WebGLDemo(env);
}

int main(int argc, char **argv)
{
  return WRun(argc, argv, &createApplication);
}

