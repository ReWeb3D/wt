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
using namespace Wt;
DrawGLWidget::DrawGLWidget(WContainerWidget *root)
    :  WGLWidget(root)
{
    textureSrc = createTextureAndLoad("./glass.png");
}


// The initializeGL() captures all JS commands that are to be executed
// before the scene is rendered for the first time. It is executed only
// once. It is re-executed when the WebGL context is restored after it
// was lost.
// In general, it should be used to set up shaders, create VBOs, initialize
// matrices, ...
DrawGLWidget::~DrawGLWidget(){
    delete (*Alpha);
    delete Alpha;
    //delete myRoot;

}

void DrawGLWidget::initializeGL()
{
    // Init variables
    Alpha = (double**)malloc(sizeof(double*));
    (*Alpha)=(double*)malloc(sizeof(double));
    Alpha[0][0]=0.5;
    useBlending=true;
    useLighting=true;
    ambientColor.assign(0.2,0.2,0.2);
    lightDirection.assign(-0.25,-0.25,-1.0);
    lightDirection.normalize();
    lightDirection.scale(-1.0);
    lightColor.assign(0.8);

    // Init Angles
    angleX = -30.0;
    velAngleX = 0.0;
    angleY = 0.0;
    velAngleY = 0.2;
    accelAngle = 0.1;
    zDepth = -5.0;




    // Init Javascript Matrix (ClientSide)
    WMatrix4x4 worldTransform;
    worldTransform.setToIdentity();
    jsMatrix_ = createJavaScriptMatrix4();
    setJavaScriptMatrix4(jsMatrix_, worldTransform);

    // Init Texture pointer


    initShaders();
    initBuffers();
    initTextures();




    viewport(0, 0, 500, 500);
    clearColor(0, 0, 0, 1.0);


}

void DrawGLWidget::animate(){}



void DrawGLWidget::initTextures(){
    texture = createTexture(); //ok
    bindTexture(TEXTURE_2D,texture); // ok Really works!!!!!!!!!!!!!!!!!!
    pixelStorei(UNPACK_FLIP_Y_WEBGL,true);// Works
    texImage2D(TEXTURE_2D,0,RGBA,RGBA,UNSIGNED_BYTE,textureSrc); // Works
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
    useBlendingUniform_  = getUniformLocation(shaderProgram_, "uAlpha");
}


void DrawGLWidget::paintGL()
{
    if(useBlending){
        blendFunc(SRC_ALPHA,ONE); // Note: GLenum ONE is not defined for blending functions!!.
        enable(BLEND);
        disable(DEPTH_TEST);
        //depthFunc(GREATER);
        uniform1f(useBlendingUniform_,Alpha[0][0]);
    }else{
        disable(BLEND);
        enable(DEPTH_TEST);
        uniform1f(useBlendingUniform_,1.0);
    }

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

    WMatrix4x4 rotMatrix;
    rotMatrix.rotate(angleX,-1.0, 0.0, 0.0);
    rotMatrix.rotate(angleY, 0.0, 1.0, 0.0);

    uniformMatrix4(cMatrixUniform_, rotMatrix);
    //set model-view matrix
    uniformMatrix4(mvMatrixUniform_, mvMatrix);

    WMatrix4x4 normalsMatrix;
    normalsMatrix = rotMatrix.inverted().transposed();

    uniformMatrix4(nMatrixUniform_, normalsMatrix);



    activeTexture(TEXTURE0);
    bindTexture(TEXTURE_2D,texture);
    uniform1i(samplerUniform_,0);


    uniform1i(useLightingUniform_,useLighting);
    uniform3f(ambientColorUniform_,ambientColor.values[0],ambientColor.values[1],ambientColor.values[2]);
    uniform3f(lightingDirectionUniform_,lightDirection.values[0],lightDirection.values[1],lightDirection.values[2]);
    uniform3f(directionalColorUniform_,lightColor.values[0],lightColor.values[1],lightColor.values[2]);



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
    bindBuffer(ARRAY_BUFFER, cubeVertexNormalBuffer);
    vertexAttribPointer(vertexNormalAttribute_,
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
    drawElements(TRIANGLES,6*2*3,UNSIGNED_SHORT,0);
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
void DrawGLWidget::updateBoolsAndAlpha(bool newUseLight,bool newUseBlend){
    useBlending=newUseBlend;
    useLighting=newUseLight;
}

void DrawGLWidget::updateLights(macVec3 newAmbientColor,macVec3 newLightDirection,macVec3 newLightColor,double **newAlpha)
{
    ambientColor=newAmbientColor;
    lightDirection=newLightDirection;
    lightColor=newLightColor;
    lightDirection.normalize();
    lightDirection.scale(-1.0);
    Alpha = newAlpha;
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


    cubeVertexNormalBuffer = createBuffer();
    bindBuffer(ARRAY_BUFFER,cubeVertexNormalBuffer);
    double cubeNormals[] = {
        // Front face
        0.0,  0.0,  1.0,
        0.0,  0.0,  1.0,
        0.0,  0.0,  1.0,
        0.0,  0.0,  1.0,
        // Back face
        0.0,  0.0, -1.0,
        0.0,  0.0, -1.0,
        0.0,  0.0, -1.0,
        0.0,  0.0, -1.0,
        // Top face
        0.0,  1.0,  0.0,
        0.0,  1.0,  0.0,
        0.0,  1.0,  0.0,
        0.0,  1.0,  0.0,
        // Bottom face
        0.0, -1.0,  0.0,
        0.0, -1.0,  0.0,
        0.0, -1.0,  0.0,
        0.0, -1.0,  0.0,
        // Right face
        1.0,  0.0,  0.0,
        1.0,  0.0,  0.0,
        1.0,  0.0,  0.0,
        1.0,  0.0,  0.0,
        // Left face
       -1.0,  0.0,  0.0,
       -1.0,  0.0,  0.0,
       -1.0,  0.0,  0.0,
       -1.0,  0.0,  0.0};
    bufferDatafv(ARRAY_BUFFER,cubeNormals,cubeNormals+3*4*6, STATIC_DRAW);

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
















