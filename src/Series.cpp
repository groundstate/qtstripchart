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

#include <QDebug>
#include <QFile>

#include "PlottingAttributes.h"
#include "Series.h"

Series::Series(int npts)
{
	x = new double[npts];
	y = new double[npts];
	xOffset = 0;
	yOffset = 0;
	
	maxPoints_=npts;
	head_=-1;
	tail_=0;
	numPoints_=0;

	pa_= new PlottingAttributes();
	
}

Series::~Series()
{
	delete[] x;
	delete[] y;
}

void Series::setColour(QColor col)
{
	pa_->colour=col;
}

QColor & Series::colour()
{
	return pa_->colour;
}

void Series::addPoint(double xt,double yt)
{
	// stored in a ring buffer
	
	head_++;
	if (numPoints_ == maxPoints_)
	{
		tail_++;
		if (head_==maxPoints_)
			head_=0;
		if (tail_==maxPoints_)
			tail_=0;
		
		x[head_]=xt;
		y[head_]=yt;
	}
	else
	{
		x[head_]=xt;
		y[head_]=yt;
		numPoints_++; 
	}	

}


int Series::head()
{
	return head_;
}

int Series::tail()
{
	return tail_;
}

void Series::setMaxPoints(int npts)
{
	delete[] x;
	delete[] y;
	
	x = new double[npts];
	y = new double[npts];

	maxPoints_=npts;
	head_=-1;
	tail_=0;
	numPoints_=0;
	id_=-1;
}

int Series::maxPoints()
{
	return maxPoints_;
}

int Series::numPoints()
{
	return numPoints_;
}

void Series::save(QString fname)
{
	QFile file(fname);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		return;
	QTextStream out(&file);
		
	if (numPoints_==0){
		file.close();
		return;
	}
	
	int stop =head_;
	if (head_ < tail_)
		stop= maxPoints_-1;
	for (int i=tail_;i<=stop;i++){
	}
	if (tail_!=0){
		for (int i=0;i<=head_;i++){
		}
	}
	file.close();
}

void Series::load(QString)
{
}
		