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

#include "VicomMatrixStack.h"

VicomJavaScriptMatrixStack::VicomJavaScriptMatrixStack(){
    matrixStack_=NULL;

    rotAngle_=NULL;

    rotVecX_=NULL;
    rotVecY_=NULL;
    rotVecZ_=NULL;

    transX_=NULL;
    transY_=NULL;
    transZ_=NULL;
}

VicomJavaScriptMatrixStack::~VicomJavaScriptMatrixStack(){
    free(matrixStack_);

    free(rotAngle_);

    free(rotVecX_);
    free(rotVecY_);
    free(rotVecZ_);

    free(transX_);
    free(transY_);
    free(transZ_);

}

void VicomJavaScriptMatrixStack::addItem(Wt::WGLWidget::JavaScriptMatrix4x4 **myPtr, Wt::WGLWidget::JavaScriptMatrix4x4 newItem){

    Wt::WGLWidget::JavaScriptMatrix4x4 *copy;

    //Copy the previous elements in order to create a new stack.
    copy = *myPtr;
    // Ask for the memory.
    (*myPtr) = (Wt::WGLWidget::JavaScriptMatrix4x4*)malloc(sizeof(Wt::WGLWidget::JavaScriptMatrix4x4)*(numel_+1));
    //Copy the previous elements to the recently allocated stack.
    if(numel_ != 0){
        //Copy the previous elements to the recently allocated stack.
        for(int i = 0; i<numel_;i++){
            (*myPtr)[i]=copy[i];
        }
        //Free the previous memory.
        free(copy);
    }
    //Add the new item
    (*myPtr)[numel_]=newItem;
}

void VicomJavaScriptMatrixStack::addItem(double **myPtr, double newItem){

    double *copy;

    //Copy the previous elements in order to create a new stack.
    copy = *myPtr;
    // Ask for the memory.
    (*myPtr) = (double*)malloc(sizeof(double)*(numel_+1));
    //Copy the previous elements to the recently allocated stack.
    if(numel_ != 0){
        //Copy the previous elements to the recently allocated stack.
        for(int i = 0; i<numel_;i++){
            (*myPtr)[i]=copy[i];
        }
        //Free the previous memory.
        free(copy);
    }
    //Add the new item
    (*myPtr)[numel_]=newItem;
}

void VicomJavaScriptMatrixStack::push(Wt::WGLWidget::JavaScriptMatrix4x4 &m,
    double rotAngle,double rotVecX,double rotVecY,double rotVecZ,
    double transX,double transY,double transZ){

    addItem(&matrixStack_,m);
    addItem(&rotAngle_,rotAngle);
    addItem(&rotVecX_,rotVecX);
    addItem(&rotVecY_,rotVecY);
    addItem(&rotVecZ_,rotVecZ);

    addItem(&transX_,transX);
    addItem(&transY_,transY);
    addItem(&transZ_,transZ);

    numel_++;
}
