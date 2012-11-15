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


#ifndef VICOMMATRIXSTACK_H
#define VICOMMATRIXSTACK_H

#include<Wt/WGLWidget>






class VicomJavaScriptMatrixStack
{

public:
    VicomJavaScriptMatrixStack();
    ~VicomJavaScriptMatrixStack();

    void addItem(Wt::WGLWidget::JavaScriptMatrix4x4 **myPtr,Wt::WGLWidget::JavaScriptMatrix4x4 newItem);
    void addItem(double **myPtr,double newItem);

    void push(Wt::WGLWidget::JavaScriptMatrix4x4 &m,
              double rotAngle,double rotVecX,double rotVecY,double rotVecZ,
              double transX,double transY,double transZ);

private:
    Wt::WGLWidget::JavaScriptMatrix4x4 *matrixStack_;

    //Rotation Data.
    double *rotAngle_;
    double *rotVecX_;
    double *rotVecY_;
    double *rotVecZ_;

    //Translation Data.
    double *transX_;
    double *transY_;
    double *transZ_;

    //Number of elments.
    int numel_;


};



#endif // VICOMMATRIXSTACK_H
