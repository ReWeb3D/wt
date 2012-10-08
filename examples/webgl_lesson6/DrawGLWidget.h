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

class DrawGLWidget : public Wt::WGLWidget
{
public:
    DrawGLWidget(Wt::WContainerWidget *root);

  // Specialization of WGLWidgeT::intializeGL()
  void initializeGL();
  void initBuffers();
  void initShaders();
  void initTextures();
  void paintGL();
  void animate();
  void updateMotion();
  void changeTextureFilters();
  void incrementZDepth();
  void decrementZDepth();
  void incrementRotX();
  void incrementRotY();
  void decrementRotX();
  void decrementRotY();

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

  // Uniform Locations
  UniformLocation pMatrixUniform_;
  UniformLocation mvMatrixUniform_;
  UniformLocation samplerUniform_;
  UniformLocation cMatrixUniform_;


  // A client-side JavaScript matrix variable --IMPORTANT::: CLIENT-SIDE!
  JavaScriptMatrix4x4 jsMatrix_;
  JavaScriptMatrix4x4 rotMatrix;

  // The so-called VBOs, Vertex Buffer Objects
  // This one contains both vertex (xyz) and normal (xyz) data

  Buffer cubeVertexPositionBuffer;
  Buffer cubeVertexIndexBuffer;
  Buffer cubeTextureCoordBuffer;

  Wt::WContainerWidget *myRoot;

  //WImage *imag;
  double accelAngle;
  double angleX;
  double velAngleX;
  double angleY;
  double velAngleY;
  double zDepth;
  int textureIndex;
  //Wt::WImage *imag1;

  Texture textureSrc;
  Texture texture0;
  Texture texture1;
  Texture texture2;
  Texture *texture;


  // Client side matrix to control

};



#endif // DRAWGLWIDGET_H
