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

#ifndef DRAWGLWIDGET_H
#define DRAWGLWIDGET_H

#include <Wt/WGLWidget>
#include <Wt/WImage>
typedef struct {
    double values[3];
    double length(){
        return sqrt(values[0]*values[0]+
                    values[1]*values[1]+
                    values[2]*values[2]);
    };
    void normalize(){
        scale(1.0/length());
    };
    void assign(double a,double b,double c){
        values[0] = a;
        values[1] = b;
        values[2] = c;
    };
    void assign(double a){
        values[0] = a;
        values[1] = a;
        values[2] = a;
    };
    void scale(double s){
           values[0] *= s;
           values[1] *= s;
           values[2] *= s;
    };
    void scale(double q, double r,double s){
           values[0] *= q;
           values[1] *= r;
           values[2] *= s;
    };

} macVec3;


class DrawGLWidget : public Wt::WGLWidget
{
public:
    DrawGLWidget(Wt::WContainerWidget *root);
    ~DrawGLWidget();

  // Specialization of WGLWidgeT::intializeGL()
  void initializeGL();
  void initBuffers();
  void initShaders();
  void initTextures();
  void paintGL();
  void animate();
  void updateMotion();
  void incrementZDepth();
  void decrementZDepth();
  void incrementRotX();
  void incrementRotY();
  void decrementRotX();
  void decrementRotY();
  void updateLights(macVec3 newAmbientColor,macVec3 newLightDirection,macVec3 newLightColor,double **newAlpha);
  void updateBoolsAndAlpha(bool newUseLight,bool newUseBlend);

  // Sets the shader source. Must be set before the widget is first rendered.
  void setShaders(const std::string &vertexShader,
      const std::string &fragmentShader);


private:
  // The shaders, in plain text format
  std::string vertexShader_;
  std::string fragmentShader_;

  // Program and related variables
  Program shaderProgram_;

  // Atribute locations
  AttribLocation vertexPositionAttribute_;
  AttribLocation textureCoordAttribute_;
  AttribLocation vertexNormalAttribute_;

  // Uniform Locations
  UniformLocation pMatrixUniform_;
  UniformLocation mvMatrixUniform_;
  UniformLocation samplerUniform_;
  UniformLocation cMatrixUniform_;
  UniformLocation nMatrixUniform_;
  UniformLocation ambientColorUniform_;
  UniformLocation lightingDirectionUniform_;
  UniformLocation directionalColorUniform_;
  UniformLocation useLightingUniform_;
  UniformLocation useBlendingUniform_;


  // A client-side JavaScript matrix variable --IMPORTANT::: CLIENT-SIDE!
  JavaScriptMatrix4x4 jsMatrix_;
  JavaScriptMatrix4x4 rotMatrix;

  // The so-called VBOs, Vertex Buffer Objects
  // This one contains both vertex (xyz) and normal (xyz) data

  Buffer cubeVertexPositionBuffer;
  Buffer cubeVertexIndexBuffer;
  Buffer cubeTextureCoordBuffer;
  Buffer cubeVertexNormalBuffer;

  Wt::WContainerWidget *myRoot;

  //WImage *imag;
  double accelAngle;
  double angleX;
  double velAngleX;
  double angleY;
  double velAngleY;
  double zDepth;
  double **Alpha;


  bool useLighting;
  bool useBlending;
  macVec3 lightDirection;
  macVec3 lightColor;
  macVec3 ambientColor;

  Texture textureSrc;
  Texture texture;


  // Client side matrix to control

};



#endif // DRAWGLWIDGET_H
