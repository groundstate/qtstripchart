//
// qtstripchart - a stripchart widget libary for Qt
//
// The MIT License (MIT)
//
// Copyright (c)  2014  Michael J. Wouters
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#ifndef __SERIES_H_
#define __SERIES_H_

#include <QColor>
#include <QString>

#include "PlottingAttributes.h"

class Series
{
	public:
	
		Series(int npts=100000);
		~Series();

		void setID(int i){id_=i;}
		int id(){return id_;}

		void setName(QString n){name_=n;}
		QString name(){return name_;}

		void setColour(QColor );
		QColor & colour();
		
		void setPlottingAttributes(PlottingAttributes &pa){*pa_=pa;}
		PlottingAttributes &plottingAttributes(){return *pa_;}
		
		void addPoint(double,double);

		double *x;// direct access to the ring buffers
		double *y;
		double xOffset; // offset to subtract 
		double yOffset; // offset to subtract
		
		int head();
		int tail();
		void setMaxPoints(int);
		int maxPoints();
		int numPoints();

		void save(QString);
		void load(QString);
		
	private:

		int maxPoints_;
		int head_;
		int tail_;
		int numPoints_;
		QString name_;
		
		int id_;
		
		PlottingAttributes *pa_;
};

#endif
