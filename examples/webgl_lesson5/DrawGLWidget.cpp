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

#include "DrawGLWidget.h"

#include <Wt/WGLWidget>
#include <Wt/WMatrix4x4>
#include <Wt/WImage>
using namespace Wt;

DrawGLWidget::DrawGLWidget(WContainerWidget *root,WImage *im)
    :  WGLWidget(root)
{
    imag1=im;
    texture2 = createTextureAndLoad("./vicomLogoSm.png");
}


// The initializeGL() captures all JS commands that are to be executed
// before the scene is rendered for the first time. It is executed only
// once. It is re-executed when the WebGL context is restored after it
// was lost.
// In general, it should be used to set up shaders, create VBOs, initialize
// matrices, ...

void DrawGLWidget::initializeGL()
{
    WMatrix4x4 worldTransform;
    worldTransform.setToIdentity();

    jsMatrix_ = createJavaScriptMatrix4();
    setJavaScriptMatrix4(jsMatrix_, worldTransform);

    initShaders();
    initBuffers();
    initTextures();
    angle=0.0;
    viewport(0, 0, 500, 500);
    clearColor(0, 0, 0, 1.0);
    enable(DEPTH_TEST);

}

void DrawGLWidget::animate(){}


void DrawGLWidget::initTextures(){
    texture = createTexture(); //ok
    bindTexture(TEXTURE_2D,texture); // ok Really works!!!!!!!!!!!!!!!!!!
    pixelStorei(UNPACK_FLIP_Y_WEBGL,true);// Works
    texImage2D(TEXTURE_2D,0,RGBA,RGBA,UNSIGNED_BYTE,texture2); // Works
    texParameteri(TEXTURE_2D,TEXTURE_MAG_FILTER,NEAREST);
    texParameteri(TEXTURE_2D,TEXTURE_MIN_FILTER,NEAREST);


}
void DrawGLWidget::initShaders()
{

    Shader fragmentShader = createShader(FRAGMENT_SHADER);
    
    shaderSource(fragmentShader, fragmentShader_);
    compileShader(fragmentShader);
    Shader vertexShader = createShader(VERTEX_SHADER);
    shaderSource(vertexShader, vertexShader_);
    compileShader(vertexShader);
    shaderProgram_ = createProgram();
    attachShader(shaderProgram_, vertexShader);
    attachShader(shaderProgram_, fragmentShader);
    linkProgram(shaderProgram_);
    useProgram(shaderProgram_);

    // Extract the references to the attributes from the shader.
    vertexPositionAttribute_ =
      getAttribLocation(shaderProgram_, "aVertexPosition");
    enableVertexAttribArray(vertexPositionAttribute_);

    textureCoordAttribute_ =
      getAttribLocation(shaderProgram_, "aTextureCoord");
    enableVertexAttribArray(textureCoordAttribute_);

    // Extract the references the uniforms from the shader
    pMatrixUniform_  = getUniformLocation(shaderProgram_, "uPMatrix");
    mvMatrixUniform_ = getUniformLocation(shaderProgram_, "uMVMatrix");
    cMatrixUniform_ =  getUniformLocation(shaderProgram_, "uCMatrix");
    samplerUniform_  = getUniformLocation(shaderProgram_, "uSampler");
}


void DrawGLWidget::paintGL()
{
    clear(COLOR_BUFFER_BIT | DEPTH_BUFFER_BIT);
    // Reset Z-buffer, enable Z-buffering

    // Draw Cube
    // set perspective matrix
    WMatrix4x4 pMatrix;
    pMatrix.perspective(45,1,0.1,100.0);
    uniformMatrix4(pMatrixUniform_, pMatrix);
    //set model-view matrix
    WMatrix4x4 mvMatrix;
    mvMatrix.setToIdentity();
    mvMatrix.translate(0.0,0.0,-5.0);

    WMatrix4x4 rotMatrix;
    rotMatrix.rotate(0.72,1.0,1.0,1.0);
    jsMatrix_ = (jsMatrix_ * rotMatrix);

    //mvMatrix = (mvMatrix * jsMatrix_);
    uniformMatrix4(cMatrixUniform_, jsMatrix_);
    // set perspective matrix
    //set model-view matrix
    uniformMatrix4(mvMatrixUniform_, mvMatrix);

    activeTexture(TEXTURE0);
    bindTexture(TEXTURE_2D,texture);
    uniform1i(samplerUniform_,0);

    bindBuffer(ARRAY_BUFFER, cubeVertexPositionBuffer);
    vertexAttribPointer(vertexPositionAttribute_,
        3,     // size: Every vertex has an X, Y and Z component
        FLOAT, // type: They are floats
        false, // normalized: Please, do NOT normalize the vertices
        0, // stride: The first byte of the next vertex is located this
        //         amount of bytes further. The format of the VBO is
        //         vx, vy, vz, nx, ny, nz and every element is a
        //         Float32, hence 4 bytes large
        0);    // offset: The byte position of the first vertex in the buffer

    bindBuffer(ARRAY_BUFFER, cubeTextureCoordBuffer);
    vertexAttribPointer(textureCoordAttribute_,
        2,     // size: Every vertex has RGBA components
        FLOAT, // type: They are floats
        false, // normalized: Please, do NOT normalize the vertices
        0, // stride: The first byte of the next vertex is located this
        //         amount of bytes further. The format of the VBO is
        //         vx, vy, vz, nx, ny, nz and every element is a
        //         Float32, hence 4 bytes large
        0);    // offset: The byte position of the first vertex in the buffer*/
    bindBuffer(ELEMENT_ARRAY_BUFFER, cubeVertexIndexBuffer);
    drawElements(TRIANGLES,6*2*3,UNSIGNED_SHORT,0); // What are you going to draw, Offset , Number of elements

    this->repaintGL(PAINT_GL);
}


void DrawGLWidget::setShaders(const std::string &vertexShader,
    const std::string &fragmentShader)
{
  vertexShader_ = vertexShader;
  fragmentShader_ = fragmentShader;

}

void DrawGLWidget::initBuffers()
{
    // Define the geometry of the Cube
    cubeVertexPositionBuffer = createBuffer();
    bindBuffer(ARRAY_BUFFER,cubeVertexPositionBuffer);
    double cubeVertices[] = {
        // Front face
        -1.0, -1.0,  1.0,
         1.0, -1.0,  1.0,
         1.0,  1.0,  1.0,
        -1.0,  1.0,  1.0,
        // Back face
        -1.0, -1.0, -1.0,
        -1.0,  1.0, -1.0,
         1.0,  1.0, -1.0,
         1.0, -1.0, -1.0,
        // Top face
        -1.0,  1.0, -1.0,
        -1.0,  1.0,  1.0,
         1.0,  1.0,  1.0,
         1.0,  1.0, -1.0,
        // Bottom face
        -1.0, -1.0, -1.0,
         1.0, -1.0, -1.0,
         1.0, -1.0,  1.0,
        -1.0, -1.0,  1.0,
        // Right face
         1.0, -1.0, -1.0,
         1.0,  1.0, -1.0,
         1.0,  1.0,  1.0,
         1.0, -1.0,  1.0,
        // Left face
        -1.0, -1.0, -1.0,
        -1.0, -1.0,  1.0,
        -1.0,  1.0,  1.0,
        -1.0,  1.0, -1.0};
    bufferDatafv(ARRAY_BUFFER,cubeVertices,cubeVertices+3*4*6, STATIC_DRAW);

    cubeTextureCoordBuffer = createBuffer();
    bindBuffer(ARRAY_BUFFER,cubeTextureCoordBuffer);
    double cubeTexCoord[] = {
        // Front face
        0.0, 0.0,
        1.0, 0.0,
        1.0, 1.0,
        0.0, 1.0,
        // Back face
        1.0, 0.0,
        1.0, 1.0,
        0.0, 1.0,
        0.0, 0.0,
        // Top face
        0.0, 1.0,
        0.0, 0.0,
        1.0, 0.0,
        1.0, 1.0,
        // Bottom face
        1.0, 1.0,
        0.0, 1.0,
        0.0, 0.0,
        1.0, 0.0,
        // Right face
        1.0, 0.0,
        1.0, 1.0,
        0.0, 1.0,
        0.0, 0.0,
        // Left face
        0.0, 0.0,
        1.0, 0.0,
        1.0, 1.0,
        0.0, 1.0};
    bufferDatafv(ARRAY_BUFFER,cubeTexCoord,cubeTexCoord+2*4*6, STATIC_DRAW);

    cubeVertexIndexBuffer = createBuffer();
    bindBuffer(ELEMENT_ARRAY_BUFFER,cubeVertexIndexBuffer);
    unsigned short int cubeIndices[] = {
                0, 1, 2,      0, 2, 3,    // Front face
                4, 5, 6,      4, 6, 7,    // Back face
                8, 9, 10,     8, 10, 11,  // Top face
                12, 13, 14,   12, 14, 15, // Bottom face
                16, 17, 18,   16, 18, 19, // Right face
                20, 21, 22,   20, 22, 23  // Left face
            };


    bufferDataiv(ELEMENT_ARRAY_BUFFER,cubeIndices,cubeIndices+6*2*3,STATIC_DRAW,UNSIGNED_SHORT);
}
















