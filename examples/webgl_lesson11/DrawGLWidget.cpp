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
#include <stdio.h>
#include <Wt/WHTML5Video>
#include "VicomMatrixStack.h"
using namespace Wt;
DrawGLWidget::DrawGLWidget(WContainerWidget *root)
    :  WGLWidget(root)
{
    textureSrc = createTextureAndLoad("./moon.gif");




}


// The initializeGL() captures all JS commands that are to be executed
// before the scene is rendered for the first time. It is executed only
// once. It is re-executed when the WebGL context is restored after it
// was lost.
// In general, it should be used to set up shaders, create VBOs, initialize
// matrices, ...

void DrawGLWidget::initializeGL()
{
    // Init variables
    useLighting=true;
    ambientColor.assign(0.2,0.2,0.2);
    lightDirection.assign(-1.0,-1.0,-1.0);
    lightDirection.normalize();
    lightDirection.scale(-1.0);
    lightColor.assign(0.8);

    // Init Angles
    angleX = -30.0;
    velAngleX = 0.0;
    angleY = 0.0;
    velAngleY = 0.0;
    accelAngle = 0.1;
    zDepth = -6.0;


    // Init Javascript Matrix (ClientSide)
    WMatrix4x4 worldTransform;
    worldTransform.setToIdentity();
    jsMatrix_ = createJavaScriptMatrix4();
    setJavaScriptMatrix4(jsMatrix_, worldTransform);

    rotMatrix=createJavaScriptMatrix4();
    //setClientSideWalkHandler(rotMatrix,0.01,0.01);
    setClientSideLookAtHandler(rotMatrix,0,0,0,0,1,0,0.007,0.007);
    // Init Texture pointer


    initShaders();
    initBuffers();
    initTextures();




    viewport(0, 0, 500, 500);
    clearColor(0, 0, 0, 1.0);
    enable(DEPTH_TEST);

}

void DrawGLWidget::animate(){}



void DrawGLWidget::initTextures(){
    texture = createTexture(); //ok
    bindTexture(TEXTURE_2D,texture); // ok Really works!!!!!!!!!!!!!!!!!!
    pixelStorei(UNPACK_FLIP_Y_WEBGL,true);// Works
    texImage2D(TEXTURE_2D,0,RGBA,RGBA,UNSIGNED_BYTE,textureSrc); //
    texParameteri(TEXTURE_2D,TEXTURE_MAG_FILTER,LINEAR);// ok
    texParameteri(TEXTURE_2D,TEXTURE_MIN_FILTER,LINEAR_MIPMAP_NEAREST);//ok
    generateMipmap(TEXTURE_2D);
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

    vertexNormalAttribute_ =
      getAttribLocation(shaderProgram_, "aVertexNormal");
    enableVertexAttribArray(vertexNormalAttribute_);



    // Extract the references the uniforms from the shader MATRICES
    pMatrixUniform_  = getUniformLocation(shaderProgram_, "uPMatrix");
    mvMatrixUniform_ = getUniformLocation(shaderProgram_, "uMVMatrix");
    cMatrixUniform_ =  getUniformLocation(shaderProgram_, "uCMatrix");
    nMatrixUniform_ =  getUniformLocation(shaderProgram_, "uNMatrix");


    samplerUniform_  = getUniformLocation(shaderProgram_, "uSampler");

    // Extract the references the uniforms from the shader MATRICES
    ambientColorUniform_  = getUniformLocation(shaderProgram_, "uAmbientColor");
    lightingDirectionUniform_  = getUniformLocation(shaderProgram_, "uLightingDirection");
    directionalColorUniform_  = getUniformLocation(shaderProgram_, "uDirectionalColor");
    useLightingUniform_  = getUniformLocation(shaderProgram_, "uUseLighting");
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
    mvMatrix.translate(0.0,0.0,zDepth);

    //WMatrix4x4 rotMatrix;
    //rotMatrix.rotate(angleX,-1.0, 0.0, 0.0);
    //rotMatrix.rotate(angleY, 0.0, 1.0, 0.0);

    uniformMatrix4(cMatrixUniform_, rotMatrix);
    //set model-view matrix
    uniformMatrix4(mvMatrixUniform_, mvMatrix);
    //
    uniformMatrix4(nMatrixUniform_, rotMatrix.inverted().transposed());



    activeTexture(TEXTURE0);
    bindTexture(TEXTURE_2D,texture);
    uniform1i(samplerUniform_,0);


    uniform1i(useLightingUniform_,useLighting);
    uniform3f(ambientColorUniform_,ambientColor.values[0],ambientColor.values[1],ambientColor.values[2]);
    uniform3f(lightingDirectionUniform_,lightDirection.values[0],lightDirection.values[1],lightDirection.values[2]);
    uniform3f(directionalColorUniform_,lightColor.values[0],lightColor.values[1],lightColor.values[2]);

    bindBuffer(ARRAY_BUFFER, cubeVertexPositionBuffer);
    vertexAttribPointer(vertexPositionAttribute_,
        cubeVertices.itemsize_,     // size: Every vertex has an X, Y and Z component
        FLOAT, // type: They are floats
        false, // normalized: Please, do NOT normalize the vertices
        0, // stride: The first byte of the next vertex is located this
        //         amount of bytes further. The format of the VBO is
        //         vx, vy, vz, nx, ny, nz and every element is a
        //         Float32, hence 4 bytes large
        0);    // offset: The byte position of the first vertex in the buffer
    bindBuffer(ARRAY_BUFFER, cubeVertexNormalBuffer);
    vertexAttribPointer(vertexNormalAttribute_,
        cubeNormals.itemsize_,     // size: Every vertex has an X, Y and Z component
        FLOAT, // type: They are floats
        false, // normalized: Please, do NOT normalize the vertices
        0, // stride: The first byte of the next vertex is located this
        //         amount of bytes further. The format of the VBO is
        //         vx, vy, vz, nx, ny, nz and every element is a
        //         Float32, hence 4 bytes large
        0);    // offset: The byte position of the first vertex in the buffer
    bindBuffer(ARRAY_BUFFER, cubeTextureCoordBuffer);
    vertexAttribPointer(textureCoordAttribute_,
        cubeTexCoord.itemsize_,     // size: Every vertex has RGBA components
        FLOAT, // type: They are floats
        false, // normalized: Please, do NOT normalize the vertices
        0, // stride: The first byte of the next vertex is located this
        //         amount of bytes further. The format of the VBO is
        //         vx, vy, vz, nx, ny, nz and every element is a
        //         Float32, hence 4 bytes large
        0);    // offset: The byte position of the first vertex in the buffer*/
    bindBuffer(ELEMENT_ARRAY_BUFFER, cubeVertexIndexBuffer);
    drawElements(TRIANGLES,cubeIndices.numel_,UNSIGNED_SHORT,0);
}

void DrawGLWidget::updateMotion(){
    angleX += velAngleX;
    angleY += velAngleY;
}

void DrawGLWidget::incrementZDepth(){
    zDepth += accelAngle;
}
void DrawGLWidget::decrementZDepth(){
    zDepth -= accelAngle;
}


void DrawGLWidget::incrementRotX(){
    velAngleX += accelAngle;
}
void DrawGLWidget::incrementRotY(){
    velAngleY += accelAngle;
}
void DrawGLWidget::decrementRotX(){
    velAngleX -= accelAngle;
}
void DrawGLWidget::decrementRotY(){
    velAngleY -= accelAngle;
}

void DrawGLWidget::setShaders(const std::string &vertexShader,
    const std::string &fragmentShader)
{
  vertexShader_ = vertexShader;
  fragmentShader_ = fragmentShader;

}


void DrawGLWidget::updateLights(macVec3 newAmbientColor,macVec3 newLightDirection,macVec3 newLightColor)
{
    ambientColor=newAmbientColor;
    lightDirection=newLightDirection;
    lightColor=newLightColor;
    lightDirection.normalize();
    lightDirection.scale(-1.0);
}

void DrawGLWidget::initBuffers()
{
    // Define the geometry of the Cube
    int latitudeBands=30;
    int longitudeBands =30;
    double radius = 2.0;



    cubeVertices.initialize(3);
    cubeTexCoord.initialize(2);
    cubeNormals.initialize(3);
    cubeIndices.initialize(3);

    double pi=3.1415926535;

    for (int latNumber=0; latNumber <= latitudeBands; latNumber++) {
        double theta = latNumber * pi / latitudeBands;
        double sinTheta = sin(theta);
        double cosTheta = cos(theta);

        for (int longNumber=0; longNumber <= longitudeBands; longNumber++) {
            double phi = longNumber * 2 * pi / longitudeBands;
            double sinPhi = sin(phi);
            double cosPhi = cos(phi);

            double x = cosPhi * sinTheta;
            double y = cosTheta;
            double z = sinPhi * sinTheta;
            double u = 1 - ((double)longNumber / longitudeBands);
            double v = 1 - ((double)latNumber / latitudeBands);

            cubeNormals.push(x);
            cubeNormals.push(y);
            cubeNormals.push(z);
            cubeTexCoord.push(u);
            cubeTexCoord.push(v);
            cubeVertices.push(radius * x);
            cubeVertices.push(radius * y);
            cubeVertices.push(radius * z);
        }
    }

    for (int latNumber=0; latNumber < latitudeBands; latNumber++) {
        for (int longNumber=0; longNumber < longitudeBands; longNumber++) {
            int first = (latNumber * (longitudeBands + 1)) + longNumber;
            int second = first + longitudeBands + 1;
            cubeIndices.push(first);
            cubeIndices.push(second);
            cubeIndices.push(first + 1);

            cubeIndices.push(second);
            cubeIndices.push(second + 1);
            cubeIndices.push(first + 1);
        }
    }


    cubeVertexPositionBuffer = createBuffer();
    bindBuffer(ARRAY_BUFFER,cubeVertexPositionBuffer);
    bufferDatafv(ARRAY_BUFFER,cubeVertices.start(),cubeVertices.end(), STATIC_DRAW);


    cubeVertexNormalBuffer = createBuffer();
    bindBuffer(ARRAY_BUFFER,cubeVertexNormalBuffer);
    bufferDatafv(ARRAY_BUFFER,cubeNormals.start(),cubeNormals.end(), STATIC_DRAW);

    cubeTextureCoordBuffer = createBuffer();
    bindBuffer(ARRAY_BUFFER,cubeTextureCoordBuffer);
    bufferDatafv(ARRAY_BUFFER,cubeTexCoord.start(),cubeTexCoord.end(), STATIC_DRAW);

    cubeVertexIndexBuffer = createBuffer();
    bindBuffer(ELEMENT_ARRAY_BUFFER,cubeVertexIndexBuffer);
    bufferDataiv(ELEMENT_ARRAY_BUFFER,cubeIndices.start(),cubeIndices.end(),STATIC_DRAW,UNSIGNED_SHORT);

    cubeNormals.finalize();
    cubeTexCoord.finalize();
    cubeVertices.finalize();
    cubeIndices.finalize();


}
















