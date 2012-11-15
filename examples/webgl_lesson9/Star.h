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

#ifndef STAR_H
#define STAR_H

#include <Wt/WGLWidget>
#include <Wt/WMatrix4x4>
#include <Wt/WImage>
#include <stdio.h>

class Star{
public:
    Star();
    Star(double startingDistance, double newRotationSpeed);
    ~Star();

    void animate();
    void randomiseColors();
    Wt::WMatrix4x4 rotateTilt(const Wt::WMatrix4x4 mvMatrixGlobal,double tilt,bool twinkle);
    Wt::WMatrix4x4 rotateSpin(const Wt::WMatrix4x4 mvMatrixGlobal,double tilt,double spin,bool twinkle);
    void drawStar(Wt::WMatrix4x4 mvMatrix);

    double getR(){return this->r;};
    double getG(){return this->g;};
    double getB(){return this->b;};

    double getTwinkleR(){return this->twinkleR;};
    double getTwinkleG(){return this->twinkleG;};
    double getTwinkleB(){return this->twinkleB;};



private:

    double angle;
    double dist;
    double rotationSpeed;
    double twinkleR;
    double twinkleG;
    double twinkleB;
    double r;
    double g;
    double b;
};



#endif // STAR_H
