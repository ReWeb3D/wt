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


class DrawGLWidget : public Wt::WGLWidget
{
public:
  DrawGLWidget(Wt::WContainerWidget *root);

  // Specialization of WGLWidgeT::intializeGL()
  void initializeGL();
  void initBuffers();
  void initShaders();
  void paintGL();
  void animate();

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
  AttribLocation vertexColorAttribute_;

  // Uniform Locations
  UniformLocation pMatrixUniform_;
  UniformLocation mvMatrixUniform_;

  // A client-side JavaScript matrix variable --IMPORTANT::: CLIENT-SIDE!
  JavaScriptMatrix4x4 jsMatrix_;
  JavaScriptMatrix4x4 rotMatrix;

  // The so-called VBOs, Vertex Buffer Objects
  // This one contains both vertex (xyz) and normal (xyz) data

  Buffer pyramidVertexPositionBuffer;
  Buffer cubeVertexPositionBuffer;
  Buffer pyramidVertexColorBuffer;
  Buffer cubeVertexColorBuffer;
  Buffer cubeVertexIndexBuffer;

  double angle;

};



#endif // DRAWGLWIDGET_H
