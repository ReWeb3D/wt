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


#include <Wt/WGLWidget>
#include <Wt/WMatrix4x4>
#include <Wt/WImage>
#include <stdio.h>
#include "DrawGLWidget.h"
#include "Star.h"

Star::Star(){}
Star::Star(double startingDistance, double newRotationSpeed){
    this->angle = 0.0;
    this->dist = startingDistance;
    this->rotationSpeed = newRotationSpeed;

    // Set the colors to its starting value.
    this->randomiseColors();
}

Star::~Star(){}



void Star::animate(){
    this->angle += this->rotationSpeed;

    //decrease distance and reset the star if necessary.
    this->dist-=0.01;
    if(this->dist<=0.0){
        this->dist+=5.0;
        this->randomiseColors();
    }
}

void Star::randomiseColors(){
    // Give the star a random color for nomal
    // circumstances.
    this->r = ((double)rand())/RAND_MAX;
    this->g = ((double)rand())/RAND_MAX;
    this->b = ((double)rand())/RAND_MAX;

    // Set the twinkle values.
    this->twinkleR = ((double)rand())/RAND_MAX;
    this->twinkleG = ((double)rand())/RAND_MAX;
    this->twinkleB = ((double)rand())/RAND_MAX;
}

void Star::drawStar(Wt::WMatrix4x4 mvMatrix){
}

Wt::WMatrix4x4 Star::rotateTilt(const Wt::WMatrix4x4 mvMatrixGlobal,
                double tilt,bool twinkle){

    Wt::WMatrix4x4 mvMatrix = Wt::WMatrix4x4(mvMatrixGlobal);

    mvMatrix.rotate(this->angle,0.0,1.0,0.0);
    mvMatrix.translate(this->dist,0.0,0.0);
    mvMatrix.rotate(-this->angle,0.0,1.0,0.0);
    mvMatrix.rotate(-tilt,1.0,0.0,0.0);

    return mvMatrix;


    // return mvMatrix.rotate(spin,0.0,0.0,1.0);


}

Wt::WMatrix4x4 Star::rotateSpin(const Wt::WMatrix4x4 mvMatrixGlobal,
                double tilt,double spin,bool twinkle){

    Wt::WMatrix4x4 mvMatrix = Wt::WMatrix4x4(mvMatrixGlobal);

    mvMatrix = rotateTilt(mvMatrix,tilt,twinkle);

    mvMatrix.rotate(spin,0.0,0.0,1.0);

    return mvMatrix;


    // return mvMatrix.rotate(spin,0.0,0.0,1.0);


}
