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
#include "Star.h"

using namespace Wt;
DrawGLWidget::DrawGLWidget(WContainerWidget *root)
    :  WGLWidget(root),numStars(50)
{
    textureSrc = createTextureAndLoad("./star.png");
}


// The initializeGL() captures all JS commands that are to be executed
// before the scene is rendered for the first time. It is executed only
// once. It is re-executed when the WebGL context is restored after it
// was lost.
// In general, it should be used to set up shaders, create VBOs, initialize
// matrices, ...
DrawGLWidget::~DrawGLWidget(){
    delete[] stars;
    //delete myRoot;

}

void DrawGLWidget::initializeGL()
{
    // Init variables
    stars = new Star[numStars]();

    for(int i=0;i<numStars;i++){
        stars[i]=Star((5.0*(double)i)/numStars,((double)i)/numStars);
    }

    // Init Angles
    angleX = 0.0;
    velAngleX = 0.0;
    angleY = 90;
    velAngleY = 0.0;
    accelAngle = 0.1;
    zDepth = -15.0;
    spin=0.0;




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
    texParameteri(TEXTURE_2D,TEXTURE_MIN_FILTER,LINEAR);//ok
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





    // Extract the references the uniforms from the shader MATRICES
    pMatrixUniform_  = getUniformLocation(shaderProgram_, "uPMatrix");
    mvMatrixUniform_ = getUniformLocation(shaderProgram_, "uMVMatrix");
    //cMatrixUniform_  = getUniformLocation(shaderProgram_, "uCMatrix");
    //nMatrixUniform_  = getUniformLocation(shaderProgram_, "uNMatrix");


    samplerUniform_  = getUniformLocation(shaderProgram_, "uSampler");

    // Extract the references the uniforms from the shader MATRICES
    colorUniform_  = getUniformLocation(shaderProgram_, "uColor");
}


void DrawGLWidget::paintGL()
{

    blendFunc(SRC_ALPHA,ONE); // Note: GLenum ONE is not defined for blending functions!!.
    enable(BLEND);
    disable(DEPTH_TEST);

    clear(COLOR_BUFFER_BIT | DEPTH_BUFFER_BIT);
    // Reset Z-buffer, enable Z-buffering

    // Draw Cube
    // set perspective matrix
    WMatrix4x4 pMatrix;
    pMatrix.perspective(45,1,0.1,100.0);

    //set model-view matrix
    WMatrix4x4 mvMatrix;
    mvMatrix.setToIdentity();
    mvMatrix.translate(0.0,0.0,zDepth);
    mvMatrix.rotate(angleY,1.0,0.0,0.0);
    WMatrix4x4 mvMatrixParticular;

    for(int i=0;i<numStars;i++){
        if (useTwinkle){
            uniform3f(colorUniform_,stars[i].getTwinkleB(),stars[i].getTwinkleG(),stars[i].getTwinkleB());

            uniformMatrix4(pMatrixUniform_, pMatrix);
            uniformMatrix4(mvMatrixUniform_, mvMatrixParticular);

            activeTexture(TEXTURE0);
            bindTexture(TEXTURE_2D,texture);
            uniform1i(samplerUniform_,0);

            bindBuffer(ARRAY_BUFFER, starVertexPositionBuffer);
            vertexAttribPointer(vertexPositionAttribute_,
                3,     // size: Every vertex has an X, Y and Z component
                FLOAT, // type: They are floats
                false, // normalized: Please, do NOT normalize the vertices
                0, // stride: The first byte of the next vertex is located this
                //         amount of bytes further. The format of the VBO is
                //         vx, vy, vz, nx, ny, nz and every element is a
                //         Float32, hence 4 bytes large
                0);    // offset: The byte position of the first vertex in the buffer

            bindBuffer(ARRAY_BUFFER, starTextureCoordBuffer);
            vertexAttribPointer(textureCoordAttribute_,
                2,     // size: Every vertex has RGBA components
                FLOAT, // type: They are floats
                false, // normalized: Please, do NOT normalize the vertices
                0, // stride: The first byte of the next vertex is located this
                //         amount of bytes further. The format of the VBO is
                //         vx, vy, vz, nx, ny, nz and every element is a
                //         Float32, hence 4 bytes large
                0);    // offset: The byte position of the first vertex in the buffer*/

            //uniformMatrix4(mvMatrixUniform_, mvMatrix);
            drawArrays(TRIANGLE_STRIP,0,4);

        }

        mvMatrixParticular=stars[i].rotateSpin(mvMatrix,angleY,spin,true);

        uniform3f(colorUniform_,stars[i].getR(),stars[i].getG(),stars[i].getB());



        uniformMatrix4(pMatrixUniform_, pMatrix);
        uniformMatrix4(mvMatrixUniform_, mvMatrixParticular);

        activeTexture(TEXTURE0);
        bindTexture(TEXTURE_2D,texture);
        uniform1i(samplerUniform_,0);

        bindBuffer(ARRAY_BUFFER, starVertexPositionBuffer);
        vertexAttribPointer(vertexPositionAttribute_,
            3,     // size: Every vertex has an X, Y and Z component
            FLOAT, // type: They are floats
            false, // normalized: Please, do NOT normalize the vertices
            0, // stride: The first byte of the next vertex is located this
            //         amount of bytes further. The format of the VBO is
            //         vx, vy, vz, nx, ny, nz and every element is a
            //         Float32, hence 4 bytes large
            0);    // offset: The byte position of the first vertex in the buffer

        bindBuffer(ARRAY_BUFFER, starTextureCoordBuffer);
        vertexAttribPointer(textureCoordAttribute_,
            2,     // size: Every vertex has RGBA components
            FLOAT, // type: They are floats
            false, // normalized: Please, do NOT normalize the vertices
            0, // stride: The first byte of the next vertex is located this
            //         amount of bytes further. The format of the VBO is
            //         vx, vy, vz, nx, ny, nz and every element is a
            //         Float32, hence 4 bytes large
            0);    // offset: The byte position of the first vertex in the buffer*/

        //uniformMatrix4(mvMatrixUniform_, mvMatrix);
        drawArrays(TRIANGLE_STRIP,0,4);
        stars[i].animate();
        spin+=0.1;
    }



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
    velAngleY += accelAngle;
}
void DrawGLWidget::incrementRotY(){
    //velAngleY += accelAngle;
}
void DrawGLWidget::decrementRotX(){
    velAngleY -= accelAngle;
}
void DrawGLWidget::decrementRotY(){
    //velAngleY -= accelAngle;
}

void DrawGLWidget::setShaders(const std::string &vertexShader,
    const std::string &fragmentShader)
{
  vertexShader_ = vertexShader;
  fragmentShader_ = fragmentShader;

}
void DrawGLWidget::updateBools(bool newUseLight){
    useTwinkle = newUseLight;
}

void DrawGLWidget::updateLights(macVec3 newAmbientColor,macVec3 newLightDirection,macVec3 newLightColor,double **newAlpha)
{

}

void DrawGLWidget::initBuffers()
{
    // Define the geometry of the Cube
    starVertexPositionBuffer = createBuffer();
    bindBuffer(ARRAY_BUFFER,starVertexPositionBuffer);
    double starVertices[] = {
        -1.0, -1.0,  0.0,
         1.0, -1.0,  0.0,
        -1.0,  1.0,  0.0,
         1.0,  1.0,  0.0};
    bufferDatafv(ARRAY_BUFFER,starVertices,starVertices+3*4, STATIC_DRAW);

    starTextureCoordBuffer = createBuffer();
    bindBuffer(ARRAY_BUFFER,starTextureCoordBuffer);
    double starTexCoord[] = {
        0.0, 0.0,
        1.0, 0.0,
        0.0, 1.0,
        1.0, 1.0};
    bufferDatafv(ARRAY_BUFFER,starTexCoord,starTexCoord+2*4, STATIC_DRAW);
}
















