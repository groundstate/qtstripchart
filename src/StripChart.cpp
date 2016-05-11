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

#include <QLabel>
#include <QLayout>

#include "Axis.h"
#include "AxisWidget.h"
#include "Plotter.h"
#include "PlottingAttributes.h"
#include "Series.h"
#include "StripChart.h"

StripChart::StripChart(QWidget *parent):QFrame(parent)
{

	QGridLayout *gl = new QGridLayout(this);
	
	title_=new QLabel("Title",this);
	gl->addWidget(title_,0,1);
	
	int frameWidth=3;
	
	QFrame *chartFrame= new QFrame(this);
	chartFrame->setFrameStyle(QFrame::Panel | QFrame::Sunken);
	chartFrame->setLineWidth(frameWidth);
	gl->addWidget(chartFrame,2,1);
	
	QVBoxLayout *vb = new QVBoxLayout(chartFrame);
	vb->setContentsMargins(0,0,0,0);
	plotter_ = new Plotter(this);
	plotter_->setScrollingWindow(true);
	
	vb->addWidget(plotter_);

	xAxis_ = new AxisWidget(frameWidth,plotter_->xAxis(),this);
	xAxis_->setOrientation(Qt::Horizontal);
	xAxis_->setTitle("time (s)");
	gl->addWidget(xAxis_,3,1);
	
	yAxis_ = new AxisWidget(frameWidth,plotter_->yAxis(),this);
	yAxis_->setOrientation(Qt::Vertical);
	yAxis_->setTitle("data (arb.)");
	gl->addWidget(yAxis_,2,0);

	gl->setSpacing(0);
	gl->setRowStretch(0,0);
	gl->setRowStretch(1,0);
	gl->setRowStretch(2,1);
	gl->setRowStretch(3,0);
	
	gl->setColumnStretch(0,0);
	gl->setColumnStretch(1,1);
	
	connect(plotter_,SIGNAL(updating()),this,SLOT(setUpdating()));
	connect(plotter_,SIGNAL(inspecting()),this,SLOT(setInspecting()));
	
	connect(xAxis_,SIGNAL(rangeChanged(double,double)),this,SLOT(updateXAxisRange(double,double)));
	connect(yAxis_,SIGNAL(rangeChanged(double,double)),this,SLOT(updateYAxisRange(double,double)));
	
}

StripChart::~StripChart()
{
}

Series* StripChart::addSeries(QString n)
{
	Series *d = plotter_->addSeries(n);
	return d;
}

void StripChart::addPoint(int id,double x,double y)
{
	plotter_->addPoint(id,x,y);
	update();
}

void StripChart::addPoints(int *id,double *x,double *y,int npoints)
{
	plotter_->addPoints(id,x,y,npoints);	
	update();
}

void StripChart::setTitle(QString t)
{
	title_->setText(t);
	if (t.length()==0) title_->hide();
}

void StripChart::bindAxisWidget(AxisWidget *aw)
{
	switch(aw->orientation()){
		case Qt::Horizontal:
			connect(aw,SIGNAL(rangeChanged(double,double)),this,SLOT(updateXAxisRange(double,double)));
			break;
		case Qt::Vertical:
			connect(aw,SIGNAL(rangeChanged(double,double)),this,SLOT(updateYAxisRange(double,double)));
			break;
	}
			
}

void StripChart::save(QString)
{
}

void StripChart::load(QString)
{
}
		
//
//
//

void StripChart::setUpdating()
{
	//scroller_->setEnabled(false);
}

void StripChart::setInspecting()
{
	//scroller_->setEnabled(true);
}

void StripChart::updateXAxisRange(double xmin,double xmax)
{
	//plotter_->yAxis()->setAutoScale(false);
	plotter_->setXWindow(xmin,xmax);
	plotter_->setScrollingWindow(xmax-xmin);
	plotter_->update();
}

void StripChart::updateYAxisRange(double ymin,double ymax)
{
	plotter_->yAxis()->setAutoScale(false);
	plotter_->yAxis()->setRange(ymin,ymax);
	plotter_->update();
}


// void StripChart::updateReadout(double xd ,double yd)
// {
// 	QString l =xAxis_->formatValue(xd);
// 	xReadout_->setText(l);
// 	l =yAxis_->formatValue(yd);
// 	yReadout_->setText(l);
// }



