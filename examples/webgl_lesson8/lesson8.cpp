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
#include <Wt/WLineEdit>
#include <Wt/WCheckBox>
//#include <Wt/WPushButton>
#include "DrawGLWidget.h"
#include <stdio.h>


using namespace Wt;

const std::string fragShad(
"#ifdef GL_ES \n"
"precision highp float; \n"
"#endif \n"
" \n"
"varying vec2 vTextureCoord; \n"
"varying vec3 vLightWeighting; \n"
" \n"
"uniform sampler2D uSampler; \n"
"uniform float uAlpha; \n"
" \n"
"void main(void) { \n"
"    vec4 textureColor = texture2D(uSampler, vec2(vTextureCoord.s, vTextureCoord.t)); \n"
"    gl_FragColor = vec4(textureColor.rgb * vLightWeighting, textureColor.a*uAlpha); \n"
"    //gl_FragColor = vec4(vLightWeighting,1.0); \n"
"} \n");


const std::string vertShad(
"attribute vec3 aVertexPosition; \n"
"attribute vec3 aVertexNormal; \n"
"attribute vec2 aTextureCoord; \n"
" \n"
"uniform mat4 uMVMatrix; \n"
"uniform mat4 uPMatrix; \n"
"uniform mat4 uNMatrix; \n"
"uniform mat4 uCMatrix; \n"
" \n"
"uniform vec3 uAmbientColor; \n"
" \n"
"uniform vec3 uLightingDirection; \n"
"uniform vec3 uDirectionalColor; \n"
" \n"
"uniform bool uUseLighting; \n"
" \n"
"varying vec2 vTextureCoord; \n"
"/**/ varying vec3 vVertexNormal; \n"
"varying vec3 vLightWeighting; \n"
" \n"
"void main(void) { \n"
"    gl_Position = uPMatrix * uMVMatrix * uCMatrix *vec4(aVertexPosition, 1.0); \n"
"    vTextureCoord = aTextureCoord; \n"
"    /**/ //vVertexNormal=aVertexNormal; \n"
"    vec3 transformedNormal; \n"
"    if (!uUseLighting) { \n"
"        vLightWeighting = vec3(1.0, 1.0, 1.0); \n"
"    } else { \n"
"        transformedNormal = (uNMatrix * vec4(aVertexNormal,0.0)).xyz; \n"
"        float directionalLightWeighting = max(dot(transformedNormal, uLightingDirection), 0.0); \n"
"        vLightWeighting = uAmbientColor + uDirectionalColor * directionalLightWeighting; \n"
"    } \n"
"    vVertexNormal  =   transformedNormal;\n"
"} \n");


/*
 * A pretty basic WebGL demo application
 */
class WebGLDemo : public WApplication
{
public:
  WebGLDemo(const WEnvironment& env);
  ~WebGLDemo();
  void animate();
  void eventKeyWentDown(const WKeyEvent& event);
  double getVal(std::string str);
  void eventKeyWentUp(const WKeyEvent& event);
  void eventCheckBoxChanged();

private:
  void updateShaders();
  void resetShaders();
  double **newAlpha;

  WContainerWidget *glContainer_;
  DrawGLWidget *drawGLWidget_;
  WTimer *timer;
  WEvent *event;
  WImage *wario;

  WLineEdit *lightDirX;
  WLineEdit *lightDirY;
  WLineEdit *lightDirZ;

  WLineEdit *lightColR;
  WLineEdit *lightColG;
  WLineEdit *lightColB;

  WLineEdit *ambLightColR;
  WLineEdit *ambLightColG;
  WLineEdit *ambLightColB;

  WLineEdit *texAlpha;

  macVec3 lightColor;
  macVec3 lightDirection;
  macVec3 ambientColor;

  WCheckBox *useLighting;
  WCheckBox *useBlending;
};

WebGLDemo::~WebGLDemo(){
    delete (*newAlpha);
    delete newAlpha;

    delete glContainer_;
    delete drawGLWidget_;
    delete timer;
    delete event;
    delete wario;

    delete lightDirX;
    delete lightDirY;
    delete lightDirZ;

    delete lightColR;
    delete lightColG;
    delete lightColB;

    delete ambLightColR;
    delete ambLightColG;
    delete ambLightColB;

    delete texAlpha;
    delete useLighting;
    delete useBlending;
}

void WebGLDemo::eventKeyWentDown(const WKeyEvent& event)
{
    Key e = event.key();
    if (e ==  Key_PageDown ) {// change if "avpag" is pressed.
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
    newAlpha = (double**)malloc(sizeof(double*));
    (*newAlpha)=(double*)malloc(sizeof(double));
  setTitle("Demo");

  root()->addWidget(new WText("Wt/WebGL Lesson 8: The Depth Buffer, Transparency and Blending"));
  root()->addWidget(new WBreak());
  drawGLWidget_ = 0;

  glContainer_ = new WContainerWidget(root());
  glContainer_->resize(500, 500);
  glContainer_->setInline(false);
  //wario=new WImage("http://192.168.65.15:8080/thredds/wms?dataset=http://www.esrl.noaa.gov/psd/thredds/dodsC/Datasets/godas/ucur.2010.nc&LAYERS=ucur&ELEVATION=-5&TIME=2010-12-01T00:00:00.000Z&TRANSPARENT=false&STYLES=boxfill/greyscale&CRS=EPSG:4326&COLORSCALERANGE=-3,3&NUMCOLORBANDS=200&LOGSCALE=false&SERVICE=WMS&VERSION=1.3.0&REQUEST=GetMap&EXCEPTIONS=XML&FORMAT=image/png&BBOX=-180,-90,180,90&WIDTH=512&HEIGHT=512",root());


  root()->addWidget(new WBreak());
  root()->addWidget(new WText("Rotate with up/down/left/right keys and zoom with page up/page down"));
  root()->addWidget(new WBreak());

  root()->addWidget(new WText(" "));
  root()->addWidget(new WBreak());
  root()->addWidget(new WText("USE BLENDING"));
  root()->addWidget(new WBreak());
  useBlending=new WCheckBox("Put it transparent",root());
  useBlending->setChecked();
  root()->addWidget(new WBreak());
  root()->addWidget(new WText("Alpha Value"));
  texAlpha = new WLineEdit("0.5",root());


  root()->addWidget(new WText(" "));
  root()->addWidget(new WBreak());
  root()->addWidget(new WText("USE LIGHTING"));
  root()->addWidget(new WBreak());
  useLighting=new WCheckBox("Turn off/off lights",root());
  useLighting->setChecked();




  root()->addWidget(new WText(" "));
  root()->addWidget(new WBreak());
  root()->addWidget(new WText("LIGHT DIRECTION"));
  root()->addWidget(new WBreak());
  root()->addWidget(new WText("X : "));
  lightDirX = new WLineEdit("-0.25",root());
  root()->addWidget(new WText(" Y : "));
  lightDirY = new WLineEdit("-0.25",root());
  root()->addWidget(new WText(" Z : "));
  lightDirZ = new WLineEdit("-1.0",root());

  root()->addWidget(new WBreak());
  root()->addWidget(new WText(" "));
  root()->addWidget(new WBreak());
  root()->addWidget(new WText("LIGHT COLOR"));
  root()->addWidget(new WBreak());

  root()->addWidget(new WText("R : "));
  lightColR = new WLineEdit("0.8",root());
  root()->addWidget(new WText(" G : "));
  lightColG = new WLineEdit("0.8",root());
  root()->addWidget(new WText(" B : "));
  lightColB = new WLineEdit("0.8",root());


  root()->addWidget(new WBreak());
  root()->addWidget(new WText(" "));
  root()->addWidget(new WBreak());
  root()->addWidget(new WText("AMBIENT LIGHT COLOR"));
  root()->addWidget(new WBreak());
  root()->addWidget(new WText("R : "));
  ambLightColR = new WLineEdit("0.2",root());
  root()->addWidget(new WText(" G : "));
  ambLightColG = new WLineEdit("0.2",root());
  root()->addWidget(new WText(" B : "));
  ambLightColB = new WLineEdit("0.2",root());



  lightDirX->keyWentUp().connect(this,&WebGLDemo::eventKeyWentUp);
  lightDirY->keyWentUp().connect(this,&WebGLDemo::eventKeyWentUp);
  lightDirZ->keyWentUp().connect(this,&WebGLDemo::eventKeyWentUp);

  lightColR->keyWentUp().connect(this,&WebGLDemo::eventKeyWentUp);
  lightColG->keyWentUp().connect(this,&WebGLDemo::eventKeyWentUp);
  lightColB->keyWentUp().connect(this,&WebGLDemo::eventKeyWentUp);

  ambLightColR->keyWentUp().connect(this,&WebGLDemo::eventKeyWentUp);
  ambLightColG->keyWentUp().connect(this,&WebGLDemo::eventKeyWentUp);
  ambLightColB->keyWentUp().connect(this,&WebGLDemo::eventKeyWentUp);

      texAlpha->keyWentUp().connect(this,&WebGLDemo::eventKeyWentUp);
  useBlending->changed().connect(this,&WebGLDemo::eventCheckBoxChanged);
  useLighting->changed().connect(this,&WebGLDemo::eventCheckBoxChanged);




  updateShaders();


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



double WebGLDemo::getVal(std::string str){

    std::stringstream stream;
    double a;

    stream<<str;
    stream>>a;
    return a;
}

void WebGLDemo::eventKeyWentUp(const WKeyEvent& event){
    lightColor.assign(getVal(lightColR->text().toUTF8()),
                      getVal(lightColG->text().toUTF8()),
                      getVal(lightColB->text().toUTF8()) );
    lightDirection.assign(getVal(lightDirX->text().toUTF8()),
                          getVal(lightDirY->text().toUTF8()),
                          getVal(lightDirZ->text().toUTF8()) );
    ambientColor.assign(getVal(ambLightColR->text().toUTF8()),
                        getVal(ambLightColG->text().toUTF8()),
                        getVal(ambLightColB->text().toUTF8()) );

    newAlpha[0][0] = getVal(texAlpha->text().toUTF8());

    drawGLWidget_->updateLights(ambientColor,lightDirection,lightColor,newAlpha);
    drawGLWidget_->repaintGL(DrawGLWidget::PAINT_GL);

}

void WebGLDemo::eventCheckBoxChanged() {
    drawGLWidget_->updateBoolsAndAlpha(useLighting->isChecked(),
                                       useBlending->isChecked() );

    drawGLWidget_->repaintGL(DrawGLWidget::PAINT_GL);


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
}

WApplication *createApplication(const WEnvironment& env)
{
  return new WebGLDemo(env);
}

int main(int argc, char **argv)
{
  return WRun(argc, argv, &createApplication);
}

