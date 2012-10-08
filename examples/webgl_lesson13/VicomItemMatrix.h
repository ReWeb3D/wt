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

#ifndef VICOMITEMMATRIX_H
#define VICOMITEMMATRIX_H


#include<Wt/WGLWidget>
#include <stdio.h>

template<typename vicomT>

class VicomItemMatrix{
public:
    VicomItemMatrix(int itemsize);
    ~VicomItemMatrix();

    VicomItemMatrix &push(vicomT val);

    vicomT *start(){
        return Array_;
    }

    vicomT *end(){
        return start()+numel_;
    }

    vicomT *Array_;
    int numel_;
    int itemsize_;
};



template<typename vicomT>
VicomItemMatrix<vicomT>::VicomItemMatrix(int itemsize){
    Array_ = NULL;
    numel_=0;
    itemsize_=itemsize;
}


template<typename vicomT>
VicomItemMatrix<vicomT>::~VicomItemMatrix(){
    free(Array_);
}


template<typename vicomT>
VicomItemMatrix<vicomT> &VicomItemMatrix<vicomT>::push(vicomT val){
    vicomT *Copy_;
    Copy_=Array_;

    Array_ = (vicomT*)malloc(sizeof(vicomT)*(numel_+1));
    if (numel_ != 0){
        for(int i=0;i<numel_;i++){
            Array_[i]=Copy_[i];
        }
    }
    Array_[numel_]=val;
    numel_++;

    free(Copy_);

    return *this;
}



#endif // VICOMITEMMATRIX_H
