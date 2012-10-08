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
"varying vec3 vTransformedNormal; \n"
"varying vec4 vPosition; \n"
" \n"
"uniform bool uUseColorMap; \n"
"uniform bool uUseSpecularMap; \n"
"uniform bool uUseLighting; \n"
" \n"
"uniform vec3 uAmbientColor; \n"
" \n"
"uniform vec3 uPointLightingLocation; \n"
"uniform vec3 uPointLightingSpecularColor; \n"
"uniform vec3 uPointLightingDiffuseColor; \n"
" \n"
"uniform sampler2D uColorMapSampler; \n"
"uniform sampler2D uSpecularMapSampler; \n"
" \n"
" \n"
"void main(void) { \n"
"    vec3 lightWeighting; \n"
"    if (!uUseLighting) { \n"
"        lightWeighting = vec3(1.0, 1.0, 1.0); \n"
"    } else { \n"
"        vec3 lightDirection = normalize(uPointLightingLocation - vPosition.xyz); \n"
"         vec3 normal = normalize(vTransformedNormal); \n"
" \n"
"        float specularLightWeighting = 0.0; \n"
"        float shininess = 32.0; \n"
"        if (uUseSpecularMap) { \n"
"            shininess = texture2D(uSpecularMapSampler, vec2(vTextureCoord.s, vTextureCoord.t)).r * 255.0; \n"
"        } \n"
"        if (shininess < 255.0) { \n"
"            vec3 eyeDirection = normalize(-vPosition.xyz); \n"
"            vec3 reflectionDirection = reflect(-lightDirection, normal); \n"
" \n"
"            specularLightWeighting = pow(max(dot(reflectionDirection, eyeDirection), 0.0), shininess); \n"
"         } \n"
" \n"
"        float diffuseLightWeighting = max(dot(normal, lightDirection), 0.0); \n"
"        lightWeighting = uAmbientColor \n"
"            + uPointLightingSpecularColor * specularLightWeighting \n"
"            + uPointLightingDiffuseColor * diffuseLightWeighting; \n"
"    } \n"
" \n"
"    vec4 fragmentColor; \n"
"    if (uUseColorMap) { \n"
"        fragmentColor = texture2D(uColorMapSampler, vec2(vTextureCoord.s, vTextureCoord.t)); \n"
"    } else { \n"
"        fragmentColor = vec4(1.0, 1.0, 1.0, 1.0); \n"
"    } \n"
"    gl_FragColor = vec4(fragmentColor.rgb * lightWeighting, fragmentColor.a); \n"
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
"varying vec2 vTextureCoord; \n"
"varying vec3 vTransformedNormal; \n"
"varying vec4 vPosition; \n"
" \n"
" \n"
"void main(void) { \n"
"    vPosition = uMVMatrix * uCMatrix *vec4(aVertexPosition, 1.0); \n"
"    gl_Position = uPMatrix * vPosition; \n"
"    vTextureCoord = aTextureCoord; \n"
"    vTransformedNormal = (uNMatrix * vec4(aVertexNormal,0.0)).xyz; \n"
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

  WLineEdit *specColR;
  WLineEdit *specColG;
  WLineEdit *specColB;


  macVec3 lightColor;
  macVec3 lightDirection;
  macVec3 ambientColor;
  macVec3 specularColor;

  WSlider *slider;
  WCheckBox *chBox1;
  WCheckBox *chBox2;
  WCheckBox *chBox3;

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

void WebGLDemo::eventChecked(){
    drawGLWidget_->updateBools(chBox1->isChecked(),chBox2->isChecked(),chBox3->isChecked());
    drawGLWidget_->repaintGL(DrawGLWidget::PAINT_GL);
}

WebGLDemo::WebGLDemo(const WEnvironment& env)
  : WApplication(env)
{
  setTitle("Demo");

  root()->addWidget(new WText("Wt/WebGL Lesson 15: specular maps"));
  root()->addWidget(new WBreak());
  drawGLWidget_ = 0;

  glContainer_ = new WContainerWidget(root());
  glContainer_->resize(500, 500);
  glContainer_->setInline(false);

  root()->addWidget(new WBreak());
  root()->addWidget(new WText("Rotate with mouse and zoom with scroll"));
  root()->addWidget(new WBreak());


  chBox1=new WCheckBox("Use Lights",root());
  chBox1->setChecked();
  chBox1->clicked().connect(this,&WebGLDemo::eventChecked);

  chBox2=new WCheckBox("Use Color Map",root());
  chBox2->setChecked();
  chBox2->clicked().connect(this,&WebGLDemo::eventChecked);


  chBox3=new WCheckBox("Use Specular Map",root());
  chBox3->setChecked();
  chBox3->clicked().connect(this,&WebGLDemo::eventChecked);



  root()->addWidget(new WText(" "));
  root()->addWidget(new WBreak());
  root()->addWidget(new WText("LIGHT SOURCE LOCATION"));
  root()->addWidget(new WBreak());
  root()->addWidget(new WText("X : "));
  lightDirX = new WLineEdit("-10.0",root());
  root()->addWidget(new WText(" Y : "));
  lightDirY = new WLineEdit("2.0",root());
  root()->addWidget(new WText(" Z : "));
  lightDirZ = new WLineEdit("-5.0",root());

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
  root()->addWidget(new WText("SPECULAR LIGHT COLOR"));
  root()->addWidget(new WBreak());

  root()->addWidget(new WText("R : "));
  specColR = new WLineEdit("5.0",root());
  root()->addWidget(new WText("G : "));
  specColG = new WLineEdit("5.0",root());
  root()->addWidget(new WText("B : "));
  specColB = new WLineEdit("5.0",root());

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

  specColR->keyWentUp().connect(this,&WebGLDemo::eventKeyWentUp);
  specColG->keyWentUp().connect(this,&WebGLDemo::eventKeyWentUp);
  specColB->keyWentUp().connect(this,&WebGLDemo::eventKeyWentUp);

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
    specularColor.assign(getVal(specColR->text().toUTF8()),
                         getVal(specColG->text().toUTF8()),
                         getVal(specColB->text().toUTF8()) );


    drawGLWidget_->updateLights(ambientColor,lightDirection,lightColor,specularColor);
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

