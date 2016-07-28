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

#ifndef __PLOTTER_H_
#define __PLOTTER_H_

#include <QWidget>

#include "Axis.h"
#include "PlottingAttributes.h"

class QSlider;

class Series;

class Plotter:public QWidget
{
	Q_OBJECT

	public:

		Plotter(QWidget *parent=0);
		~Plotter();

		Series * addSeries(QString);

		void setMaxPoints(int);
		int  maxPoints();

		void setOffsets(int,double,double);
		double xOffset(int);
		double yOffset(int);
		
		void dataLimits(double *,double *,double *,double *);

		static QColor seriesColor(int);
		QColor & gridColour(){return gridColour_;}
		void setGridColour(QColor &c){gridColour_=c;}
		
		Axis *xAxis(){return xAxis_;}
		Axis *yAxis(){return yAxis_;}
		
	public slots:
		
		void setScrollingWindow(bool);
		void setFixedWindow(double,double);
		void setXWindow(double,double);
		void setYWindow(double,double);
		
		void addPoint(int,double,double);
		void addPoints(int *,double *,double *,int);
		void attributesChanged(Series *);
		
	signals:

		void updating();
		void inspecting();
		void cursorMoved(double,double);

	protected:

		virtual void resizeEvent(QResizeEvent *);
		virtual void paintEvent(QPaintEvent *);
		virtual void mouseMoveEvent ( QMouseEvent * );
 
	private slots:

		void setUpdating();
		void setInspecting();
		void toggleGrid();

	private:

		void init();
		
		void createActions();
		void renderPoint(QPainter &,int,int,PointStyles::Enum);
		
		QList<Series *> data_;
		Axis *xAxis_,*yAxis_;
		
		double yMinData_,yMaxData_;
		double xMinData_,xMaxData_;

		double xMinWin_,xMaxWin_;
		double yMinWin_,yMaxWin_;
		
		double xMinTick_,xMaxTick_,xFirstTick_,xLastTick_,xTickSeparation_;
		double yMinTick_,yMaxTick_,yFirstTick_,yLastTick_,yTickSeparation_;
		int numXDivisions_,numYDivisions_,xScalingExponent_,yScalingExponent_;
	
		bool gridOn_;
		QColor gridColour_;
		
		bool scrolling_;
		double scrollingWindowSize_;
		
		
};

#endif
