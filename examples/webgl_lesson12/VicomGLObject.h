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

#ifndef VICOMGLOBJECT_H
#define VICOMGLOBJECT_H

#include <Wt/WGLWidget>


class VicomGLObject{
public:

    void enterVertexPositionBuffer(Wt::WGLWidget::Buffer &myBuffer,int numel, int itemsize);
    void enterVertexIndexBuffer(Wt::WGLWidget::Buffer &myBuffer,int numel, int itemsize);
    void enterTextureCoordBuffer(Wt::WGLWidget::Buffer &myBuffer,int numel, int itemsize);
    void enterVertexNormalBuffer(Wt::WGLWidget::Buffer &myBuffer,int numel, int itemsize);

    Wt::WGLWidget::Buffer VPBuffer_;//VertexPositionBuffer;
    Wt::WGLWidget::Buffer VIBuffer_;//VertexIndexBuffer;
    Wt::WGLWidget::Buffer TCBuffer_;//TextureCoordBuffer;
    Wt::WGLWidget::Buffer VNBuffer_;//VertexNormalBuffer;

    int VPBnumel_;
    int VIBnumel_;
    int TCBnumel_;
    int VNBnumel_;

    int VPBitemsize_;
    int VIBitemsize_;
    int TCBitemsize_;
    int VNBitemsize_;

};



















#endif // VICOMGLOBJECT_H
