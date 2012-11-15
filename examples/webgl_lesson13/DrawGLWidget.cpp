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
#include "VicomMatrixStack.h"
#include"VicomItemMatrix.h"
using namespace Wt;
DrawGLWidget::DrawGLWidget(WContainerWidget *root)
    :  WGLWidget(root)
{
    sphereTextureSrc = createTextureAndLoad("./moon.gif");
    cubeTextureSrc = createTextureAndLoad("./crate.png");
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
    usePerFrag_=true;
    useTextures_ = true;
    ambientColor.assign(0.0,0.2,0.2);
    lightDirection.assign(-0.2,0.0,-4.0);
    //lightDirection.normalize();
    //lightDirection.scale(-1.0);
    lightColor.assign(0.8,0.8,0.0);

    // Init Angles
    angleX = -30.0;
    velAngleX = 0.0;
    angleY = 0.0;
    velAngleY = 0.0;
    accelAngle = 0.1;
    zDepth = -5.0;


    // Init Javascript Matrix (ClientSide)
    WMatrix4x4 worldTransform;
    worldTransform.setToIdentity();
    jsMatrix_ = createJavaScriptMatrix4();
    setJavaScriptMatrix4(jsMatrix_, worldTransform);

    rotMatrix=createJavaScriptMatrix4();
    //setClientSideWalkHandler(rotMatrix,0.005,0.005);
    setClientSideLookAtHandler(rotMatrix,0,0,0,0,1,0,0.007,0.007);
    // Init Texture pointer
    setJavaScript();
    initShaders();
    initBuffers();
    initTextures();

    viewport(0, 0, 500, 500);
    clearColor(0, 0, 0, 1.0);
    enable(DEPTH_TEST);

}

void DrawGLWidget::animate(){}



void DrawGLWidget::initTextures(){
    sphereTexture = createTexture(); //ok
    bindTexture(TEXTURE_2D,sphereTexture); // ok Really works!!!!!!!!!!!!!!!!!!
    pixelStorei(UNPACK_FLIP_Y_WEBGL,true);// Works
    texImage2D(TEXTURE_2D,0,RGBA,RGBA,UNSIGNED_BYTE,sphereTextureSrc); // Works
    texParameteri(TEXTURE_2D,TEXTURE_MAG_FILTER,LINEAR);// ok
    texParameteri(TEXTURE_2D,TEXTURE_MIN_FILTER,LINEAR_MIPMAP_NEAREST);//ok
    generateMipmap(TEXTURE_2D);

    cubeTexture = createTexture(); //ok
    bindTexture(TEXTURE_2D,cubeTexture); // ok Really works!!!!!!!!!!!!!!!!!!
    pixelStorei(UNPACK_FLIP_Y_WEBGL,true);// Works
    texImage2D(TEXTURE_2D,0,RGBA,RGBA,UNSIGNED_BYTE,cubeTextureSrc); // Works
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
    //useProgram(shaderProgram_);

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
    lightingDirectionUniform_  = getUniformLocation(shaderProgram_, "uPointLightingLocation");
    directionalColorUniform_  = getUniformLocation(shaderProgram_, "uPointLightingColor");
    useLightingUniform_  = getUniformLocation(shaderProgram_, "uUseLighting");
    useTexturesUniform_  = getUniformLocation(shaderProgram_, "uUseTextures");





    Shader fragmentShader2 = createShader(FRAGMENT_SHADER);

    shaderSource(fragmentShader2, fragmentShader2_);
    compileShader(fragmentShader2);
    Shader vertexShader2 = createShader(VERTEX_SHADER);
    shaderSource(vertexShader2, vertexShader2_);
    compileShader(vertexShader2);
    shaderProgram2_ = createProgram();
    attachShader(shaderProgram2_, vertexShader2);
    attachShader(shaderProgram2_, fragmentShader2);
    linkProgram(shaderProgram2_);
    //useProgram(shaderProgram2_);

    // Extract the references to the attributes from the shader.
    vertexPositionAttribute2_ =
      getAttribLocation(shaderProgram2_, "aVertexPosition");
    enableVertexAttribArray(vertexPositionAttribute2_);

    textureCoordAttribute2_ =
      getAttribLocation(shaderProgram2_, "aTextureCoord");
    enableVertexAttribArray(textureCoordAttribute2_);

    vertexNormalAttribute2_ =
      getAttribLocation(shaderProgram2_, "aVertexNormal");
    enableVertexAttribArray(vertexNormalAttribute2_);



    // Extract the references the uniforms from the shader MATRICES
    pMatrixUniform2_  = getUniformLocation(shaderProgram2_, "uPMatrix");
    mvMatrixUniform2_ = getUniformLocation(shaderProgram2_, "uMVMatrix");
    cMatrixUniform2_ =  getUniformLocation(shaderProgram2_, "uCMatrix");
    nMatrixUniform2_ =  getUniformLocation(shaderProgram2_, "uNMatrix");


    samplerUniform2_  = getUniformLocation(shaderProgram2_, "uSampler");

    // Extract the references the uniforms from the shader MATRICES
    ambientColorUniform2_  = getUniformLocation(shaderProgram2_, "uAmbientColor");
    lightingDirectionUniform2_  = getUniformLocation(shaderProgram2_, "uPointLightingLocation");
    directionalColorUniform2_  = getUniformLocation(shaderProgram2_, "uPointLightingColor");
    useLightingUniform2_  = getUniformLocation(shaderProgram2_, "uUseLighting");
    useTexturesUniform2_  = getUniformLocation(shaderProgram2_, "uUseTextures");
}


void DrawGLWidget::paintGL()
{

    clear(COLOR_BUFFER_BIT | DEPTH_BUFFER_BIT);
    // Reset Z-buffer, enable Z-buffering
    // std::vector<Matrix4x4> matrixStack;
    // Draw Cube
    // set perspective matrix
    WMatrix4x4 pMatrix;
    WMatrix4x4 identMatrix;
    WMatrix4x4 mvMatrix;
    if (!usePerFrag_){

        useProgram(shaderProgram_);
        pMatrix.perspective(45,1,0.1,100.0);
        uniformMatrix4(pMatrixUniform_, pMatrix);
        //set model-view matrix

        mvMatrix.setToIdentity();
        mvMatrix.translate(0.0,0.0,zDepth);


        //matrixStack.push_back(mvMatrix);

        identMatrix.translate(2.0,0.0,0.0);



        uniformMatrix4(cMatrixUniform_, (rotMatrix*identMatrix));
        //set model-view matrix
        uniformMatrix4(mvMatrixUniform_, mvMatrix);
        //
        uniformMatrix4(nMatrixUniform_, (rotMatrix*identMatrix).inverted().transposed());

        activeTexture(TEXTURE0);
        bindTexture(TEXTURE_2D,sphereTexture);
        uniform1i(samplerUniform_,0);


        uniform1i(useLightingUniform_,useLighting);
        uniform1i(useTexturesUniform_,useTextures_);
        uniform3f(ambientColorUniform_,ambientColor.values[0],ambientColor.values[1],ambientColor.values[2]);
        uniform3f(lightingDirectionUniform_,lightDirection.values[0],lightDirection.values[1],lightDirection.values[2]);
        uniform3f(directionalColorUniform_,lightColor.values[0],lightColor.values[1],lightColor.values[2]);

        bindBuffer(ARRAY_BUFFER, Sphere_.VPBuffer_);
        vertexAttribPointer(vertexPositionAttribute_,
            Sphere_.VPBitemsize_,     // size: Every vertex has an X, Y and Z component
            FLOAT, // type: They are floats
            false, // normalized: Please, do NOT normalize the vertices
            0, // stride: The first byte of the next vertex is located this
            //         amount of bytes further. The format of the VBO is
            //         vx, vy, vz, nx, ny, nz and every element is a
            //         Float32, hence 4 bytes large
            0);    // offset: The byte position of the first vertex in the buffer
        bindBuffer(ARRAY_BUFFER, Sphere_.VNBuffer_);
        vertexAttribPointer(vertexNormalAttribute_,
            Sphere_.VNBitemsize_,     // size: Every vertex has an X, Y and Z component
            FLOAT, // type: They are floats
            false, // normalized: Please, do NOT normalize the vertices
            0, // stride: The first byte of the next vertex is located this
            //         amount of bytes further. The format of the VBO is
            //         vx, vy, vz, nx, ny, nz and every element is a
            //         Float32, hence 4 bytes large
            0);    // offset: The byte position of the first vertex in the buffer
        bindBuffer(ARRAY_BUFFER, Sphere_.TCBuffer_);
        vertexAttribPointer(textureCoordAttribute_,
           Sphere_.TCBitemsize_,     // size: Every vertex has RGB components
            FLOAT, // type: They are floats
            false, // normalized: Please, do NOT normalize the vertices
            0, // stride: The first byte of the next vertex is located this
            //         amount of bytes further. The format of the VBO is
            //         vx, vy, vz, nx, ny, nz and every element is a
            //         Float32, hence 4 bytes large
            0);    // offset: The byte position of the first vertex in the buffer*/
        bindBuffer(ELEMENT_ARRAY_BUFFER, Sphere_.VIBuffer_);
        drawElements(TRIANGLES,Sphere_.VIBnumel_,UNSIGNED_SHORT,0);

        uniformMatrix4(pMatrixUniform_, pMatrix);
        //set model-view matrix
        identMatrix.translate(-4.0,0.0,0.0);
        // identMatrix.scale(2.0);

        //rotMatrix.rotate(angleX,-1.0, 0.0, 0.0);
        //rotMatrix.rotate(angleY, 0.0, 1.0, 0.0);



        uniformMatrix4(cMatrixUniform_, (rotMatrix*identMatrix));
        //set model-view matrix
        uniformMatrix4(mvMatrixUniform_, mvMatrix);
        //
        uniformMatrix4(nMatrixUniform_, (rotMatrix*identMatrix).inverted().transposed());



        activeTexture(TEXTURE0);
        bindTexture(TEXTURE_2D,cubeTexture);
        uniform1i(samplerUniform_,0);


        uniform1i(useLightingUniform_,useLighting);
        uniform1i(useTexturesUniform_,useTextures_);
        uniform3f(ambientColorUniform_,ambientColor.values[0],ambientColor.values[1],ambientColor.values[2]);
        uniform3f(lightingDirectionUniform_,lightDirection.values[0],lightDirection.values[1],lightDirection.values[2]);
        uniform3f(directionalColorUniform_,lightColor.values[0],lightColor.values[1],lightColor.values[2]);

        bindBuffer(ARRAY_BUFFER, Cube_.VPBuffer_);
        vertexAttribPointer(vertexPositionAttribute_,
            Cube_.VPBitemsize_,     // size: Every vertex has an X, Y and Z component
            FLOAT, // type: They are floats
            false, // normalized: Please, do NOT normalize the vertices
            0, // stride: The first byte of the next vertex is located this
            //         amount of bytes further. The format of the VBO is
            //         vx, vy, vz, nx, ny, nz and every element is a
            //         Float32, hence 4 bytes large
            0);    // offset: The byte position of the first vertex in the buffer
        bindBuffer(ARRAY_BUFFER, Cube_.VNBuffer_);
        vertexAttribPointer(vertexNormalAttribute_,
            Cube_.VNBitemsize_,     // size: Every vertex has an X, Y and Z component
            FLOAT, // type: They are floats
            false, // normalized: Please, do NOT normalize the vertices
            0, // stride: The first byte of the next vertex is located this
            //         amount of bytes further. The format of the VBO is
            //         vx, vy, vz, nx, ny, nz and every element is a
            //         Float32, hence 4 bytes large
            0);    // offset: The byte position of the first vertex in the buffer
        bindBuffer(ARRAY_BUFFER, Cube_.TCBuffer_);
        vertexAttribPointer(textureCoordAttribute_,
           Cube_.TCBitemsize_,     // size: Every vertex has RGB components
            FLOAT, // type: They are floats
            false, // normalized: Please, do NOT normalize the vertices
            0, // stride: The first byte of the next vertex is located this
            //         amount of bytes further. The format of the VBO is
            //         vx, vy, vz, nx, ny, nz and every element is a
            //         Float32, hence 4 bytes large
            0);    // offset: The byte position of the first vertex in the buffer*/
        bindBuffer(ELEMENT_ARRAY_BUFFER, Cube_.VIBuffer_);
        drawElements(TRIANGLES,Cube_.VIBnumel_,UNSIGNED_SHORT,0);


    } else {

        useProgram(shaderProgram2_);
        pMatrix.perspective(45,1,0.1,100.0);
        uniformMatrix4(pMatrixUniform2_, pMatrix);
        //set model-view matrix

        mvMatrix.setToIdentity();
        mvMatrix.translate(0.0,0.0,zDepth);


        //matrixStack.push_back(mvMatrix);

        identMatrix.translate(2.0,0.0,0.0);



        uniformMatrix4(cMatrixUniform2_, (rotMatrix*identMatrix));
        //set model-view matrix
        uniformMatrix4(mvMatrixUniform2_, mvMatrix);
        //
        uniformMatrix4(nMatrixUniform2_, (rotMatrix*identMatrix).inverted().transposed());

        activeTexture(TEXTURE0);
        bindTexture(TEXTURE_2D,sphereTexture);
        uniform1i(samplerUniform2_,0);


        uniform1i(useLightingUniform2_,useLighting);
        uniform1i(useTexturesUniform2_,useTextures_);
        uniform3f(ambientColorUniform2_,ambientColor.values[0],ambientColor.values[1],ambientColor.values[2]);
        uniform3f(lightingDirectionUniform2_,lightDirection.values[0],lightDirection.values[1],lightDirection.values[2]);
        uniform3f(directionalColorUniform2_,lightColor.values[0],lightColor.values[1],lightColor.values[2]);

        bindBuffer(ARRAY_BUFFER, Sphere_.VPBuffer_);
        vertexAttribPointer(vertexPositionAttribute2_,
            Sphere_.VPBitemsize_,     // size: Every vertex has an X, Y and Z component
            FLOAT, // type: They are floats
            false, // normalized: Please, do NOT normalize the vertices
            0, // stride: The first byte of the next vertex is located this
            //         amount of bytes further. The format of the VBO is
            //         vx, vy, vz, nx, ny, nz and every element is a
            //         Float32, hence 4 bytes large
            0);    // offset: The byte position of the first vertex in the buffer
        bindBuffer(ARRAY_BUFFER, Sphere_.VNBuffer_);
        vertexAttribPointer(vertexNormalAttribute2_,
            Sphere_.VNBitemsize_,     // size: Every vertex has an X, Y and Z component
            FLOAT, // type: They are floats
            false, // normalized: Please, do NOT normalize the vertices
            0, // stride: The first byte of the next vertex is located this
            //         amount of bytes further. The format of the VBO is
            //         vx, vy, vz, nx, ny, nz and every element is a
            //         Float32, hence 4 bytes large
            0);    // offset: The byte position of the first vertex in the buffer
        bindBuffer(ARRAY_BUFFER, Sphere_.TCBuffer_);
        vertexAttribPointer(textureCoordAttribute2_,
           Sphere_.TCBitemsize_,     // size: Every vertex has RGB components
            FLOAT, // type: They are floats
            false, // normalized: Please, do NOT normalize the vertices
            0, // stride: The first byte of the next vertex is located this
            //         amount of bytes further. The format of the VBO is
            //         vx, vy, vz, nx, ny, nz and every element is a
            //         Float32, hence 4 bytes large
            0);    // offset: The byte position of the first vertex in the buffer*/
        bindBuffer(ELEMENT_ARRAY_BUFFER, Sphere_.VIBuffer_);
        drawElements(TRIANGLES,Sphere_.VIBnumel_,UNSIGNED_SHORT,0);

        uniformMatrix4(pMatrixUniform2_, pMatrix);
        //set model-view matrix
        identMatrix.translate(-4.0,0.0,0.0);
        // identMatrix.scale(2.0);

        uniformMatrix4(cMatrixUniform2_, (rotMatrix*identMatrix));
        //set model-view matrix
        uniformMatrix4(mvMatrixUniform2_, mvMatrix);
        //
        uniformMatrix4(nMatrixUniform2_, (rotMatrix*identMatrix).inverted().transposed());



        activeTexture(TEXTURE0);
        bindTexture(TEXTURE_2D,cubeTexture);
        uniform1i(samplerUniform2_,0);


        uniform1i(useLightingUniform2_,useLighting);
        uniform1i(useTexturesUniform2_,useTextures_);
        uniform3f(ambientColorUniform2_,ambientColor.values[0],ambientColor.values[1],ambientColor.values[2]);
        uniform3f(lightingDirectionUniform2_,lightDirection.values[0],lightDirection.values[1],lightDirection.values[2]);
        uniform3f(directionalColorUniform2_,lightColor.values[0],lightColor.values[1],lightColor.values[2]);

        bindBuffer(ARRAY_BUFFER, Cube_.VPBuffer_);
        vertexAttribPointer(vertexPositionAttribute2_,
            Cube_.VPBitemsize_,     // size: Every vertex has an X, Y and Z component
            FLOAT, // type: They are floats
            false, // normalized: Please, do NOT normalize the vertices
            0, // stride: The first byte of the next vertex is located this
            //         amount of bytes further. The format of the VBO is
            //         vx, vy, vz, nx, ny, nz and every element is a
            //         Float32, hence 4 bytes large
            0);    // offset: The byte position of the first vertex in the buffer
        bindBuffer(ARRAY_BUFFER, Cube_.VNBuffer_);
        vertexAttribPointer(vertexNormalAttribute2_,
            Cube_.VNBitemsize_,     // size: Every vertex has an X, Y and Z component
            FLOAT, // type: They are floats
            false, // normalized: Please, do NOT normalize the vertices
            0, // stride: The first byte of the next vertex is located this
            //         amount of bytes further. The format of the VBO is
            //         vx, vy, vz, nx, ny, nz and every element is a
            //         Float32, hence 4 bytes large
            0);    // offset: The byte position of the first vertex in the buffer
        bindBuffer(ARRAY_BUFFER, Cube_.TCBuffer_);
        vertexAttribPointer(textureCoordAttribute2_,
           Cube_.TCBitemsize_,     // size: Every vertex has RGB components
            FLOAT, // type: They are floats
            false, // normalized: Please, do NOT normalize the vertices
            0, // stride: The first byte of the next vertex is located this
            //         amount of bytes further. The format of the VBO is
            //         vx, vy, vz, nx, ny, nz and every element is a
            //         Float32, hence 4 bytes large
            0);    // offset: The byte position of the first vertex in the buffer*/
        bindBuffer(ELEMENT_ARRAY_BUFFER, Cube_.VIBuffer_);
        drawElements(TRIANGLES,Cube_.VIBnumel_,UNSIGNED_SHORT,0);

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
    const std::string &fragmentShader,
    const std::string &vertexShader2,
    const std::string &fragmentShader2)
{
  vertexShader_ = vertexShader;
  fragmentShader_ = fragmentShader;

  vertexShader2_ = vertexShader2;
  fragmentShader2_ = fragmentShader2;

}


void DrawGLWidget::setJavaScript(){

    std::stringstream sss;

    sss<<1;
    clientSideFcn_.setJavaScript("function WTVicomGLAnim(){if (myTickValue=-"+sss.str()+"){}"+"if (!window.requestAnimationFrame) {"+
                                 "window.requestAnimationFrame = (function() {"+
                                  "return window.requestAnimationFrame ||"+
                                 "		 window.webkitRequestAnimationFrame ||"+
                                 "		 window.mozRequestAnimationFrame ||"+
                                 "		 window.oRequestAnimationFrame ||"+
                                 "		 function(/* function FrameRequestCallback */ callback, /* DOMElement Element */ element) {"+
                                 "		   window.setTimeout(callback, 1000/60);"+
                                 "		 };"+
                                 "})();}"+
                                 " var obj = "+ "(function(){"+
                                 "var r = " + jsRef() + ";"+
                                 "var o = r ? jQuery.data(r,'obj') : null;"+
                                 "return o ? o : {ctx: null};"+
                                 "})()" +"; var ctx=obj.ctx; "+WT_CLASS ".glMatrix.mat4.rotate(" +
                                 rotMatrix.jsRef()+",0.01,[0.0,1.0,0.0],"+rotMatrix.jsRef()+");"+                                 "  obj.paintGL();"
                                 "  window.requestAnimationFrame(WTVicomGLAnim);"+
                                 "}/**Vicomtech_Client_Side_Code*/");

}

void DrawGLWidget::updateLights(macVec3 newAmbientColor,macVec3 newLightDirection,macVec3 newLightColor)
{
    ambientColor=newAmbientColor;
    lightDirection=newLightDirection;
    lightColor=newLightColor;
    //lightDirection.normalize();
    //lightDirection.scale(-1.0);
}

void DrawGLWidget::updateBools(bool usePerFrag, bool useTextures){
    usePerFrag_=usePerFrag;
    useTextures_=useTextures;

}

void DrawGLWidget::initBuffers()
{
    createSphere();
    createCube();

}



void DrawGLWidget::createSphere(){
    // Define the geometry of the sphere
    int latitudeBands=30;
    int longitudeBands =30;
    double radius = 1.0;

    Buffer sphereVertexPositionBuffer;
    Buffer sphereVertexIndexBuffer;
    Buffer sphereTextureCoordBuffer;
    Buffer sphereVertexNormalBuffer;

    VicomItemMatrix<double> sphereVertices(3);
    VicomItemMatrix<double> sphereTexCoord(2);
    VicomItemMatrix<double> sphereNormals(3);
    VicomItemMatrix<int> sphereIndices(3);

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

            sphereNormals.push(x);
            sphereNormals.push(y);
            sphereNormals.push(z);
            sphereTexCoord.push(u);
            sphereTexCoord.push(v);
            sphereVertices.push(radius * x);
            sphereVertices.push(radius * y);
            sphereVertices.push(radius * z);
        }
    }

    for (int latNumber=0; latNumber < latitudeBands; latNumber++) {
        for (int longNumber=0; longNumber < longitudeBands; longNumber++) {
            int first = (latNumber * (longitudeBands + 1)) + longNumber;
            int second = first + longitudeBands + 1;
            sphereIndices.push(first);
            sphereIndices.push(second);
            sphereIndices.push(first + 1);

            sphereIndices.push(second);
            sphereIndices.push(second + 1);
            sphereIndices.push(first + 1);
        }
    }


    sphereVertexPositionBuffer = createBuffer();
    bindBuffer(ARRAY_BUFFER,sphereVertexPositionBuffer);
    bufferDatafv(ARRAY_BUFFER,sphereVertices.start(),sphereVertices.end(), STATIC_DRAW);


    sphereVertexNormalBuffer = createBuffer();
    bindBuffer(ARRAY_BUFFER,sphereVertexNormalBuffer);
    bufferDatafv(ARRAY_BUFFER,sphereNormals.start(),sphereNormals.end(), STATIC_DRAW);

    sphereTextureCoordBuffer = createBuffer();
    bindBuffer(ARRAY_BUFFER,sphereTextureCoordBuffer);
    bufferDatafv(ARRAY_BUFFER,sphereTexCoord.start(),sphereTexCoord.end(), STATIC_DRAW);

    sphereVertexIndexBuffer = createBuffer();
    bindBuffer(ELEMENT_ARRAY_BUFFER,sphereVertexIndexBuffer);
    bufferDataiv(ELEMENT_ARRAY_BUFFER,sphereIndices.start(),sphereIndices.end(),STATIC_DRAW,UNSIGNED_SHORT);

    Sphere_.enterTextureCoordBuffer(sphereTextureCoordBuffer,sphereTexCoord.numel_,sphereTexCoord.itemsize_);
    Sphere_.enterVertexIndexBuffer(sphereVertexIndexBuffer,sphereIndices.numel_,sphereIndices.itemsize_);
    Sphere_.enterVertexNormalBuffer(sphereVertexNormalBuffer,sphereNormals.numel_,sphereNormals.itemsize_);
    Sphere_.enterVertexPositionBuffer(sphereVertexPositionBuffer,sphereVertices.numel_,sphereVertices.itemsize_);
}


void DrawGLWidget::createCube(){

    Buffer cubeVertexPositionBuffer;
    Buffer cubeVertexIndexBuffer;
    Buffer cubeTextureCoordBuffer;
    Buffer cubeVertexNormalBuffer;


    VicomItemMatrix<double> cubeVertices(3);
    VicomItemMatrix<double> cubeTexCoord(2);
    VicomItemMatrix<double> cubeNormals(3);
    VicomItemMatrix<unsigned short int> cubeIndices(3);


    // Define the geometry of the Cube
    // Front face
    cubeVertices.push(-1.0).push(-1.0).push(1.0);
    cubeVertices.push(1.0).push(-1.0).push(1.0);
    cubeVertices.push(1.0).push(1.0).push(1.0);
    cubeVertices.push(-1.0).push(1.0).push(1.0);
    // Back face
    cubeVertices.push(-1.0).push(-1.0).push(-1.0);
    cubeVertices.push(-1.0).push(1.0).push(-1.0);
    cubeVertices.push(1.0).push(1.0).push(-1.0);
    cubeVertices.push(1.0).push(-1.0).push(-1.0);
    // Top face
    cubeVertices.push(-1.0).push(1.0).push(-1.0);
    cubeVertices.push(-1.0).push(1.0).push(1.0);
    cubeVertices.push(1.0).push(1.0).push(1.0);
    cubeVertices.push(1.0).push(1.0).push(-1.0);
    // Bottom face
    cubeVertices.push(-1.0).push(-1.0).push(-1.0);
    cubeVertices.push(1.0).push(-1.0).push(-1.0);
    cubeVertices.push(1.0).push(-1.0).push(1.0);
    cubeVertices.push(-1.0).push(-1.0).push(1.0);
    // Right face
    cubeVertices.push(1.0).push(-1.0).push(-1.0);
    cubeVertices.push(1.0).push(1.0).push(-1.0);
    cubeVertices.push(1.0).push(1.0).push(1.0);
    cubeVertices.push(1.0).push(-1.0).push(1.0);
    // Left face
    cubeVertices.push(-1.0).push(-1.0).push(-1.0);
    cubeVertices.push(-1.0).push(-1.0).push(1.0);
    cubeVertices.push(-1.0).push(1.0).push(1.0);
    cubeVertices.push(-1.0).push(1.0).push(-1.0);

    // Front face
    cubeNormals.push(0.0).push(0.0).push(1.0);
    cubeNormals.push(0.0).push(0.0).push(1.0);
    cubeNormals.push(0.0).push(0.0).push(1.0);
    cubeNormals.push(0.0).push(0.0).push(1.0);
    // Back face
    cubeNormals.push(0.0).push(0.0).push(-1.0);
    cubeNormals.push(0.0).push(0.0).push(-1.0);
    cubeNormals.push(0.0).push(0.0).push(-1.0);
    cubeNormals.push(0.0).push(0.0).push(-1.0);
    // Top face
    cubeNormals.push(0.0).push(1.0).push(0.0);
    cubeNormals.push(0.0).push(1.0).push(0.0);
    cubeNormals.push(0.0).push(1.0).push(0.0);
    cubeNormals.push(0.0).push(1.0).push(0.0);
    // Bottom face
    cubeNormals.push(0.0).push(-1.0).push(0.0);
    cubeNormals.push(0.0).push(-1.0).push(0.0);
    cubeNormals.push(0.0).push(-1.0).push(0.0);
    cubeNormals.push(0.0).push(-1.0).push(0.0);
    // Right face
    cubeNormals.push(1.0).push(0.0).push(0.0);
    cubeNormals.push(1.0).push(0.0).push(0.0);
    cubeNormals.push(1.0).push(0.0).push(0.0);
    cubeNormals.push(1.0).push(0.0).push(0.0);
    // Left face
    cubeNormals.push(-1.0).push(0.0).push(0.0);
    cubeNormals.push(-1.0).push(0.0).push(0.0);
    cubeNormals.push(-1.0).push(0.0).push(0.0);
    cubeNormals.push(-1.0).push(0.0).push(0.0);

    // Front face
    cubeTexCoord.push(0.0).push(0.0);
    cubeTexCoord.push(1.0).push(0.0);
    cubeTexCoord.push(1.0).push(1.0);
    cubeTexCoord.push(0.0).push(1.0);
    // Back face
    cubeTexCoord.push(1.0).push(0.0);
    cubeTexCoord.push(1.0).push(1.0);
    cubeTexCoord.push(0.0).push(1.0);
    cubeTexCoord.push(0.0).push(0.0);
    // Top face
    cubeTexCoord.push(0.0).push(1.0);
    cubeTexCoord.push(0.0).push(0.0);
    cubeTexCoord.push(1.0).push(0.0);
    cubeTexCoord.push(1.0).push(1.0);
    // Bottom face
    cubeTexCoord.push(1.0).push(1.0);
    cubeTexCoord.push(0.0).push(1.0);
    cubeTexCoord.push(0.0).push(0.0);
    cubeTexCoord.push(1.0).push(0.0);
    // Right face
    cubeTexCoord.push(1.0).push(0.0);
    cubeTexCoord.push(1.0).push(1.0);
    cubeTexCoord.push(0.0).push(1.0);
    cubeTexCoord.push(0.0).push(0.0);
    // Left face
    cubeTexCoord.push(0.0).push(0.0);
    cubeTexCoord.push(1.0).push(0.0);
    cubeTexCoord.push(1.0).push(1.0);
    cubeTexCoord.push(0.0).push(1.0);


    cubeIndices.push( 0).push( 1).push( 2).push( 0).push( 2).push( 3);// Front face

    cubeIndices.push( 4).push( 5).push( 6).push( 4).push( 6).push( 7);// Back face

    cubeIndices.push( 8).push( 9).push(10).push( 8).push(10).push(11);// Top face

    cubeIndices.push(12).push(13).push(14).push(12).push(14).push(15);// Bottom face

    cubeIndices.push(16).push(17).push(18).push(16).push(18).push(19);// Right face

    cubeIndices.push(20).push(21).push(22).push(20).push(22).push(23);// Left face



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


    Cube_.enterTextureCoordBuffer(cubeTextureCoordBuffer,cubeTexCoord.numel_,cubeTexCoord.itemsize_);
    Cube_.enterVertexIndexBuffer(cubeVertexIndexBuffer,cubeIndices.numel_,cubeIndices.itemsize_);
    Cube_.enterVertexNormalBuffer(cubeVertexNormalBuffer,cubeNormals.numel_,cubeNormals.itemsize_);
    Cube_.enterVertexPositionBuffer(cubeVertexPositionBuffer,cubeVertices.numel_,cubeVertices.itemsize_);

}
















