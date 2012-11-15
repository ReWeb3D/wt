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




#include "VicomGLObject.h"


void VicomGLObject::enterVertexPositionBuffer(Wt::WGLWidget::Buffer &myBuffer,int numel, int itemsize){
    VPBitemsize_=itemsize;
    VPBnumel_   =numel;
    VPBuffer_   =myBuffer;
}

void VicomGLObject::enterTextureCoordBuffer(Wt::WGLWidget::Buffer &myBuffer,int numel, int itemsize){
    TCBitemsize_=itemsize;
    TCBnumel_   =numel;
    TCBuffer_   =myBuffer;
}

void VicomGLObject::enterVertexIndexBuffer(Wt::WGLWidget::Buffer &myBuffer,int numel, int itemsize){
    VIBitemsize_=itemsize;
    VIBnumel_   =numel;
    VIBuffer_   =myBuffer;
}

void VicomGLObject::enterVertexNormalBuffer(Wt::WGLWidget::Buffer &myBuffer,int numel, int itemsize){
    VNBitemsize_= itemsize;
    VNBnumel_   = numel;
    VNBuffer_   = myBuffer;
}





