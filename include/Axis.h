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

#ifndef __AXIS_H_
#define __AXIS_H_

#include <QObject>

class Axis:public QObject
{
	Q_OBJECT
	
	public:
		Axis(QObject *parent=0);
		~Axis();
		
		enum AxisStyle {Linear,Logarithmic};
		
		double min(){return min_;}
		double max(){return max_;}
		void range(double *,double *);
		double minLimit(){return absMin_;}
		double maxLimit(){return absMax_;}
		
		void setAxisStyle(AxisStyle);
		AxisStyle axisStyle(){return style_;}
		
		void setAutoScale(bool);
		bool autoScaled(){return autoScale_;}
		
		void ticks(double*,double *,double *,double *,double *,int *,int*);
		QString formatValue(double);

		void setUsesTimeOfDay(bool);

	public slots:

		void setRange(double,double);
		void setRangeLimits(double,double);
		void setMin(double);
		void setMax(double);
	
	private:

		void init();
		void setTicks();
	
		AxisStyle style_;
		
		bool autoScale_;
		double min_,max_;
		double firstTick_,lastTick_;
		double absMin_,absMax_;
		double tickSpacing_;
		int scalingExponent_;
		int nIntervals_;

		bool usesTimeOfDay_;

};


#endif

