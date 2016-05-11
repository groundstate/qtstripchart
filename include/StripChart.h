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

#ifndef __STRIPCHART_H_
#define __STRIPCHART_H_

#include <QFrame>

class QLabel;
class AxisWidget;
class Series;
class Plotter;

class StripChart:public QFrame
{
	Q_OBJECT

	public:

		StripChart(QWidget *parent=0);
		~StripChart();

		Series * addSeries(QString);
		void addPoint(int,double,double);
		void addPoints(int*, double*, double*, int);

		void setTitle(QString);
		
		Plotter * plotter(){return plotter_;} // direct access for simplicity 
		AxisWidget * xAxis(){return xAxis_;}
		AxisWidget * yAxis(){return yAxis_;}
		
		void bindAxisWidget(AxisWidget *);
		
		void save(QString);
		void load(QString);
		
	private slots:

		void setUpdating();
		void setInspecting();
		void updateXAxisRange(double,double);
		void updateYAxisRange(double,double);
		
	private:
		
		QLabel *title_;
		Plotter *plotter_;
		AxisWidget *xAxis_;
		AxisWidget *yAxis_;

};

#endif
