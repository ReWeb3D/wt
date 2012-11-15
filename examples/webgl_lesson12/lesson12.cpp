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
#include <Wt/WSlider>
#include <Wt/WPushButton>
#include "DrawGLWidget.h"
#include <stdio.h>
#include <Wt/WCheckBox>


using namespace Wt;

const std::string fragShad(
"#ifdef GL_ES \n"
"precision highp float; \n"
"#endif \n"
" \n"
"varying vec2 vTextureCoord; \n"
"varying vec3 vLightWeighting; \n"
"/**/ varying vec3 vVertexNormal; \n"
" \n"
"uniform sampler2D uSampler; \n"
" \n"
"void main(void) { \n"
"    vec4 textureColor = texture2D(uSampler, vec2(vTextureCoord.s, vTextureCoord.t)); \n"
"    gl_FragColor = vec4(textureColor.rgb * vLightWeighting, textureColor.a); \n"
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
"uniform vec3 uPointLightingLocation; \n"
"uniform vec3 uPointLightingColor; \n"
" \n"
"uniform bool uUseLighting; \n"
" \n"
"varying vec2 vTextureCoord; \n"
"varying vec3 vLightWeighting; \n"
" \n"
"void main(void) { \n"
"    vec4 mvPosition = uMVMatrix * uCMatrix *vec4(aVertexPosition, 1.0); \n"
"    gl_Position = uPMatrix * mvPosition; \n"
"    vTextureCoord = aTextureCoord; \n"
"    \n"
"    if (!uUseLighting) { \n"
"        vLightWeighting = vec3(1.0, 1.0, 1.0); \n"
"    } else { \n"
"        vec3 lightDirection = normalize(uPointLightingLocation - mvPosition.xyz); \n"
"        \n"
"        vec3 transformedNormal = (uNMatrix * vec4(aVertexNormal,0.0)).xyz; \n"
"        float directionalLightWeighting = max(dot(transformedNormal, lightDirection), 0.0); \n"
"        vLightWeighting = uAmbientColor + uPointLightingColor * directionalLightWeighting; \n"
"    } \n"
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
  void eventChecked();

private:
  void updateShaders();
  void resetShaders();

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

  macVec3 lightColor;
  macVec3 lightDirection;
  macVec3 ambientColor;

  WSlider *slider;
  WCheckBox *chBox;
};

WebGLDemo::~WebGLDemo(){
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

    delete slider;

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
  setTitle("Demo");

  root()->addWidget(new WText("Wt/WebGL Lesson 12: point lighting"));
  root()->addWidget(new WBreak());
  drawGLWidget_ = 0;

  glContainer_ = new WContainerWidget(root());
  glContainer_->resize(500, 500);
  glContainer_->setInline(false);

  root()->addWidget(new WBreak());
  root()->addWidget(new WText("Rotate with mouse and zoom with scroll"));
  root()->addWidget(new WBreak());

  chBox= new WCheckBox("Use Lighting",root());
  chBox->setChecked();
  chBox->clicked().connect(this,&WebGLDemo::eventChecked);

  root()->addWidget(new WText(" "));
  root()->addWidget(new WBreak());
  root()->addWidget(new WText("LIGHT SOURCE LOCATION"));
  root()->addWidget(new WBreak());
  root()->addWidget(new WText("X : "));
  lightDirX = new WLineEdit("0.0",root());
  root()->addWidget(new WText(" Y : "));
  lightDirY = new WLineEdit("0.0",root());
  root()->addWidget(new WText(" Z : "));
  lightDirZ = new WLineEdit("-20.0",root());

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

  updateShaders();

  this->globalKeyWentDown().connect(this,&WebGLDemo::eventKeyWentDown);
  drawGLWidget_->clientSideFcn_.exec("null","null");

  WPushButton *button = new WPushButton("Animate",root());
  button->clicked().connect(drawGLWidget_->clientSideFcn_);

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

void WebGLDemo::eventChecked(){
    drawGLWidget_->updateBools(chBox->isChecked());
    drawGLWidget_->repaintGL(DrawGLWidget::PAINT_GL);
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

    drawGLWidget_->updateLights(ambientColor,lightDirection,lightColor);
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

