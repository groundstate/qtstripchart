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

#include <sys/time.h>

#include <cmath>

#include <QDebug>

#include <QAction>
#include <QMouseEvent>
#include <QPainter>

#include "Axis.h"
#include "Plotter.h"
#include "Series.h"


// colorbrewer2.org
int PlotColors[12][3]=
{
	{141, 211, 199},
	{255, 255, 179},
	{190, 186, 218},
	{251, 128, 114},
	{128, 177, 211},
	{253, 180, 98},
	{179, 222, 105},
	{252, 205, 229},
	{217, 217, 217},
	{188, 128, 189},
	{204, 235, 197},
	{255, 237, 111}
};

#define MSIZE 3 // odd number of pixels

Plotter::Plotter(QWidget *parent):QWidget(parent)
{

	setMinimumSize(240,80);
	setCursor(Qt::CrossCursor);
	setMouseTracking(true);

	init();
	
	createActions();
	setContextMenuPolicy(Qt::ActionsContextMenu);

}

Plotter::~Plotter()
{
}

Series * Plotter::addSeries(QString n)
{
	// returns index identifying the series
	Series *d= new Series(10000);
	d->setName(n);
	data_.append(d);
	d->setID(data_.size()-1);
	return d;
}

void Plotter::setMaxPoints(int max)
{
	//data_->setMaxPoints(max);
}

int Plotter::maxPoints()
{
	//return data_->maxPoints();
	return 0;
}

void Plotter::setOffsets(int id,double xOffset,double yOffset)
{
	if (id >= data_.size()) return;
	data_[id]->xOffset=xOffset;
	data_[id]->yOffset=yOffset;
	// FIXME may need to update interanl max/mins ...
}

double Plotter::xOffset(int id)
{
	if (id >= data_.size()) return 0.0; // FIXME NOT IDEAL
	return data_[id]->xOffset;
}

double Plotter::yOffset(int id)
{
	if (id >= data_.size()) return 0.0; // FIXME NOT IDEAL
	return data_[id]->yOffset;
}

		
void Plotter::setScrollingWindow(bool)
{
	scrolling_=true;
	//scrollingWindowSize_=sz;
	//xMaxWin_=xMinWin_+scrollingWindowSize_;
	//xAxis_->setRange(xMinWin_,xMaxWin_);
	// FIXME any fixups for maxdata ?
}

void Plotter::setFixedWindow(double start,double stop)
{
	scrolling_=false;
	//xMinWin_=start;
	//xMaxWin_=stop;
	//xAxis_->setRange(xMinWin_,xMaxWin_);
}

void Plotter::setXWindow(double min,double max)
{
	xMinWin_=min;
	xMaxWin_=max;
	if (scrolling_){
		scrollingWindowSize_=max-min;
		xAxis_->setRange(xMinWin_,xMaxWin_);
		qDebug() << xMinWin_<< " " << xMaxWin_;
	}
}

void Plotter::setYWindow(double min,double max)
{
	yMinWin_=min;
	yMaxWin_=max;
	yAxis_->setRange(min,max);
}
		
void Plotter::dataLimits(double *min,double *max,double *ymin,double *ymax)
{
	if (scrolling_){
		*min=xMinData_;
		*max=xMaxData_;
	}
	else{
		//*min=xMinFixedWindow_;
		//*max=xMaxFixedWindow_;
	}	
	*ymin=yMinData_;
	*ymax=yMaxData_;

}

QColor Plotter::seriesColor(int id)
{
	int i = id % 12;

	return QColor(PlotColors[i][0],PlotColors[i][1],PlotColors[i][2]);
}


//
// Public slots
//

void Plotter::addPoint(int id,double x,double y)
{
	if (id >= data_.size()) return;
	data_[id]->addPoint(x,y);
	
	double xd = x-data_[id]->xOffset;
	
	if (xd > xMaxWin_){ // start scrolling
		xMaxWin_=xd;
		xMinWin_= xMaxWin_-scrollingWindowSize_;
	}
	
	if (xAxis_->autoScaled())
		xAxis_->setRange(xMinData_,xMaxData_);
	else
		xAxis_->setRange(xMinWin_,xMaxWin_);
	
	if (yAxis_->autoScaled())
		yAxis_->setRange(yMinData_,yMaxData_);
	
}

void Plotter::addPoints(int *id,double *x,double *y,int npoints)
{
	for (int i=0;i<npoints;i++){
		data_[id[i]]->addPoint(x[i],y[i]);
		double xd = x[i]-data_[id[i]]->xOffset;
		if (xd > xMaxWin_) // start scrolling
		{
			xMaxWin_=xd;
			xMinWin_= xMaxWin_-scrollingWindowSize_;
		}
	}
	
	if (xAxis_->autoScaled())
		xAxis_->setRange(xMinData_,xMaxData_);
	else
		xAxis_->setRange(xMinWin_,xMaxWin_);
	
	if (yAxis_->autoScaled())
		yAxis_->setRange(yMinData_,yMaxData_);
	
}

void Plotter::attributesChanged(Series *)
{
	repaint();
}


//
//	Protected:
//


void Plotter::resizeEvent(QResizeEvent *)
{
}

void Plotter::paintEvent(QPaintEvent *)
{

	struct timeval tvstart,tvstop;
	
	gettimeofday(&tvstart,NULL);
	
	QPainter p(this);
	int w = width();
	int h = height();
	p.fillRect(0,0,w,h,QBrush(QColor("black")));

	xAxis_->ticks(&xMinTick_,&xMaxTick_,&xFirstTick_,&xLastTick_,&xTickSeparation_,&numXDivisions_,&xScalingExponent_);
	yAxis_->ticks(&yMinTick_,&yMaxTick_,&yFirstTick_,&yLastTick_,&yTickSeparation_,&numYDivisions_,&yScalingExponent_);
	
	double xsf = (w-1)/(xMaxTick_-xMinTick_);
	double ysf = (h-1)/(yMaxTick_-yMinTick_);
		
	if (gridOn_)
	{
		p.setPen(gridColour_);
		
		for (int i=0;i<=numYDivisions_;i++)
		{
			int yp = rint(ysf*(yFirstTick_+i*yTickSeparation_-yMinTick_));
			p.drawLine(0,h-1-yp,w-1,h-1-yp);
		}
		for (int i=0;i<=numXDivisions_;i++)
		{
			int xp = rint(xsf*(xFirstTick_+i*xTickSeparation_-xMinTick_));
			p.drawLine(xp,0,xp,h-1);
		}
	}

	for (int j=0;j<data_.size();j++)
	{
	
		Series *cd = data_[j];
		PlottingAttributes &pa = cd->plottingAttributes();
		
		if (!pa.visible) continue;
		
		double *x=cd->x;
		double *y=cd->y;
		double xOffset = cd->xOffset;
		double yOffset = cd->yOffset;
		
		int head=cd->head();
		int npts=cd->numPoints();
		if (npts==0) continue;
		int maxpts=cd->numPoints();

		p.setPen(pa.colour);
		
		if (scrolling_) // work backwards from head
		{
			bool checktail=true;
			int lastxp = rint(xsf*(x[head]-xOffset - xMinTick_));
			int lastyp = h-1 - rint(ysf*(y[head]- yOffset - yMinTick_));
			for (int i=head-1;i>=0;i--)
			{
				if ( x[i] >= xMinWin_)
				{
					int xp = rint(xsf*(x[i]-xOffset - xMinTick_));
					int yp = h-1-rint(ysf*(y[i]- yOffset - yMinTick_));
					switch (pa.plotStyle)
					{
						case PlotStyles::Lines :
							if (lastxp != xp || lastyp != yp) 
								p.drawLine(lastxp,lastyp,xp,yp);
							break;
						case PlotStyles::Points:
							renderPoint(p,xp,yp,pa.pointStyle);
							break;
						case PlotStyles::LinesPoints:
							renderPoint(p,xp,yp,pa.pointStyle);
							p.drawLine(lastxp,lastyp,xp,yp);
							break;
						
					}
					
					lastxp=xp;
					lastyp=yp;
				}
				else
				{
					checktail=false;
					break;
				}
			}

			if (checktail && npts==maxpts)
			{
				for (int i=maxpts-1;i>head;i--)
				{
					if ( x[i] >= xMinWin_)
					{
						int xp = rint(xsf*(x[i]-xOffset-xMinTick_));
						int yp = h-1-rint(ysf*(y[i]-yOffset-yMinTick_));
						switch (pa.plotStyle)
						{
							case PlotStyles::Lines :
								if (lastxp != xp || lastyp != yp) 
									p.drawLine(lastxp,lastyp,xp,yp);break;
							case PlotStyles::Points:
								renderPoint(p,xp,yp,pa.pointStyle);
								break;
							case PlotStyles::LinesPoints:
								renderPoint(p,xp,yp,pa.pointStyle);
								p.drawLine(lastxp,lastyp,xp,yp);
								break;
						}
						lastxp=xp;
						lastyp=yp;
					}
					else
						break;
				}
			}
		}
	}
	
	gettimeofday(&tvstop,NULL);
	printf("%i %i : %i\n",(int) tvstop.tv_sec,(int) tvstop.tv_usec,(int) ((tvstop.tv_sec-tvstart.tv_sec)*1.0E6 + (tvstop.tv_usec-tvstart.tv_usec)));
}

void Plotter::mouseMoveEvent ( QMouseEvent * me)
{
	double xd=xMinTick_ + me->x()*(xMaxTick_-xMinTick_)/(width()-1.0);
	double yd=yMinTick_ + (height()-1-me->y())*(yMaxTick_-yMinTick_)/(height()-1.0);
	emit cursorMoved(xd,yd);
}

void Plotter::setUpdating()
{
	emit updating();
}

void Plotter::setInspecting()
{
	emit inspecting();
}

void Plotter::toggleGrid()
{
	gridOn_=!gridOn_;
	update();
}

//
//
//

void Plotter::init()
{
	scrolling_=true;
	scrollingWindowSize_=3000.0;

	xMinData_=xMinWin_=0;
	xMaxData_=xMaxWin_=100.0;
	yMinData_=yMinWin_=-1.1;
	yMaxData_=yMaxWin_=1.1;
		
	gridOn_=true;
	gridColour_ = QColor(0,100,0);

	xAxis_ = new Axis(this);
	xAxis_->setRange(xMinWin_,xMaxWin_);
	xAxis_->ticks(&xMinTick_,&xMaxTick_,&xFirstTick_,&xLastTick_,&xTickSeparation_,&numXDivisions_,&xScalingExponent_);
	//xAxis_->setAutoScale(true);
	
	yAxis_ = new Axis(this);
	yAxis_->setRange(yMinWin_,yMaxWin_);
	yAxis_->ticks(&yMinTick_,&yMaxTick_,&yFirstTick_,&yLastTick_,&yTickSeparation_,&numYDivisions_,&yScalingExponent_);
}

void Plotter::createActions()
{
	QActionGroup *ag = new QActionGroup(this);
	ag->setExclusive(true);

	QAction *act = new QAction(tr("Updating"),this);
	addAction(act);
	act->setCheckable(true);
	ag->addAction(act);
	act->setChecked(true);
	connect(act,SIGNAL(triggered()),this,SLOT(setUpdating()));
	
	act = new QAction(tr("Inspecting"),this);
	addAction(act);
	act->setCheckable(true);
	ag->addAction(act);
	connect(act,SIGNAL(triggered()),this,SLOT(setInspecting()));

	act = new QAction(tr("Grid"),this);
	addAction(act);
	act->setCheckable(true);
	act->setChecked(gridOn_);
	connect(act,SIGNAL(triggered()),this,SLOT(toggleGrid()));

}

void Plotter::renderPoint(QPainter &p,int xp,int yp,PointStyles::Enum ps)
{
	switch (ps)
	{
		case PointStyles::Box:
			p.drawLine(xp-MSIZE,yp-MSIZE,xp+MSIZE,yp-MSIZE);
			p.drawLine(xp-MSIZE,yp-MSIZE,xp-MSIZE,yp+MSIZE);
			p.drawLine(xp-MSIZE,yp+MSIZE,xp+MSIZE,yp+MSIZE);
			p.drawLine(xp+MSIZE,yp+MSIZE,xp+MSIZE,yp-MSIZE);
			break;
		case PointStyles::Cross:
			p.drawLine(xp-MSIZE,yp-MSIZE,xp+MSIZE,yp+MSIZE);
			p.drawLine(xp-MSIZE,yp+MSIZE,xp+MSIZE,yp-MSIZE);
			break;
		case PointStyles::Dot:
			p.drawPoint(xp,yp);
			break;
	}
}