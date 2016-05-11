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
#include <cfloat>

#include <QDebug>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QMenu>
#include <QMouseEvent>
#include <QPainter>
#include <QTextDocument>
#include <QWidgetAction>

#include "DoubleInput.h"
#include "AxisWidget.h"


AxisWidget::AxisWidget(int offset,
											 Axis *axis,
											 QWidget *parent):QWidget(parent)
{
	//setContextMenuPolicy(Qt::CustomContextMenu);
	
	init();
	axis_=axis;
	offset_=offset;
	setOrientation(orientation_);
	setContentsMargins(0,0,0,0);
	
	//connect(this,SIGNAL(customContextMenuRequested ( const QPoint &)),
	//	this,SLOT(createContextMenu(const QPoint &)));
	// FIXME QT4.7 has QStaticText for this
	title_ = new QTextDocument("axis",this);
	titleText_="axis";
}
		
AxisWidget::~AxisWidget()
{
}

void AxisWidget::setOrientation(Qt::Orientation o)
{
	orientation_=o;
	switch (o)
	{
		case Qt::Horizontal:
			setMinimumSize(240,fontMetrics().height()*2+majorTickHeight_+tickLabelOffset_);
			break;
		case Qt::Vertical:
			setMinimumSize(fontMetrics().width("E")*6+ fontMetrics().height()+majorTickHeight_+tickLabelOffset_,80);
			break;
	}
}

// FIXME stupid name cf formatNumber
QString AxisWidget::formatValue(double d)
{
	QString ret;
	if (usesTimeOfDay_)
	{
		int secs= ((int) rint(d)) % 86400;
		int hh = secs / 3600;
		int mm = (secs - hh*3600) / 60;
		int ss = secs - hh*3600 - mm*60;
		ret.sprintf("%02d:%02d:%02d",hh,mm,ss);
	}
	else
	{
		ret.sprintf("%g",d);
	}
	return ret;
}


/*!
	Set the axis title.
	The \a title can be rich text, described using HTML. See Qt's documentation for a list
	of the supported HTML tags.
*/
void AxisWidget::setTitle(QString title)
{
	// Note that t can be rich text
	title_->setHtml(title);
	titleText_=title; // we may fiddle with this so keep a copy for convenience
}


//
//
//


void AxisWidget::setMin(double min)
{
	// assuming inconsistent axis limits come from a UI event
	// shift the other axis limit to maintain the axis range
	
	axis_->setMin(min);
	title_->setHtml(titleText_);//back to default in case the axis is no longer scaled 
	emit rangeChanged(min,axis_->max());
	update();
}

void AxisWidget::setMax(double max)
{
	axis_->setMax(max);
	title_->setHtml(titleText_);//back to defaults in case the axis is no longer scaled 
	emit rangeChanged(axis_->min(),max);
	update();
}

void AxisWidget::setRange(double min,double max)
{
	axis_->setRange(min,max);
	title_->setHtml(titleText_);//back to defaults in case the axis is no longer scaled 
	// inconsistent - should emit rangeChanged
	update();
}

	
//
//	Protected
//

void AxisWidget::resizeEvent(QResizeEvent *)
{
	w_=width();
	h_=height();
	geometryDirty_=true;
}

void AxisWidget::paintEvent(QPaintEvent *)
{
	QPainter p(this);
	int w = width();
	int h = height();
	
	double min,max;
	double firstTick,lastTick;
	double tickSpacing;
	int scalingExponent;
	int nIntervals;
		
	axis_->ticks(&min,&max,&firstTick,&lastTick,&tickSpacing,&nIntervals,&scalingExponent);
	
	//p.fillRect(0,0,w,h,QBrush(QColor("yellow")));

	QPen aPen;
	aPen.setStyle(Qt::SolidLine);
	aPen.setColor(QColor(Qt::black));
	p.setPen(aPen);

	int labelHeight = fontMetrics().ascent();

	if (orientation_==Qt::Vertical)
	{
	
		int x0 = w;
		
		// min tick 
	
		p.drawLine(x0,h-1-offset_,x0-majorTickHeight_,h-1-offset_);
		QString tickLabel =formatTick(min,scalingExponent,tickSpacing);
		int labelWidth = fontMetrics().width(tickLabel,-1);
		p.drawText(x0-labelWidth-tickLabelOffset_-majorTickHeight_,
								h-labelHeight-offset_,labelWidth,labelHeight,Qt::AlignLeft | Qt::AlignVCenter,tickLabel);
		minTickLabel_.setRect(x0-labelWidth-tickLabelOffset_-majorTickHeight_,h-labelHeight-offset_,
																labelWidth,labelHeight);
		// max tick
		
		p.drawLine(x0,offset_,x0-majorTickHeight_,offset_);
		tickLabel =formatTick(max,scalingExponent,tickSpacing);
		labelWidth = fontMetrics().width(tickLabel,-1);
		p.drawText(x0-labelWidth-tickLabelOffset_-majorTickHeight_,
								offset_,labelWidth,labelHeight,Qt::AlignLeft | Qt::AlignVCenter,tickLabel);
		maxTickLabel_.setRect(x0-labelWidth-tickLabelOffset_-majorTickHeight_,offset_,
																labelWidth,labelHeight);
																
		for (int i=0;i<=nIntervals;i++)
		{

			double tickVal=firstTick+i*tickSpacing;

			int y = offset_+(int) (h-1-2*offset_)*(1-(tickVal-min)	/(max-min));
			p.drawLine(x0,y,x0-majorTickHeight_,y);

			tickLabel =formatTick(tickVal,scalingExponent,tickSpacing);
			labelWidth = fontMetrics().width(tickLabel,-1);
			// make sure label doesn't overwrite the min or max ticks
			if (i==0 && minTickLabel_.top() <= y+labelHeight/2)
				continue;
			else if (i==nIntervals && y - labelHeight/2 < maxTickLabel_.bottom())
				continue;
			p.drawText(x0-labelWidth-tickLabelOffset_-majorTickHeight_,y-labelHeight/2,labelWidth,labelHeight,Qt::AlignLeft | Qt::AlignVCenter,tickLabel);
			
		}

		// axis title

		int tw = title_->size().width();
		//int th = title_->size().height();
		p.rotate(-90.0);
		p.translate(-(h+tw)/2,0);
		title_->drawContents(&p);
		
	}
	else if (orientation_==Qt::Horizontal)
	{


		int y0=0;
		// min tick 
	
		p.drawLine(offset_,y0,offset_,y0+majorTickHeight_-1);
		QString tickLabel =formatTick(min,scalingExponent,tickSpacing);
		int labelWidth = fontMetrics().width(tickLabel,-1);
		p.drawText(offset_,y0+tickLabelOffset_+majorTickHeight_,labelWidth,labelHeight,Qt::AlignLeft | Qt::AlignTop,tickLabel);
		minTickLabel_.setRect(offset_,tickLabelOffset_+majorTickHeight_,labelWidth,labelHeight);
		// max tick
		
		p.drawLine(w-1-offset_,y0,w-1-offset_,y0+majorTickHeight_-1);
		tickLabel =formatTick(max,scalingExponent,tickSpacing);
		labelWidth = fontMetrics().width(tickLabel,-1);
		p.drawText(w-labelWidth-offset_,tickLabelOffset_+majorTickHeight_,
								labelWidth,labelHeight,Qt::AlignRight | Qt::AlignTop,tickLabel);
		maxTickLabel_.setRect(w-labelWidth-offset_,tickLabelOffset_+majorTickHeight_,labelWidth,labelHeight);

		for (int i=0;i<=nIntervals;i++)
		{

			double tickVal=firstTick+i*tickSpacing;

			int x = offset_+(int) (w-1-2*offset_)*((tickVal-min)/(max-min));
			p.drawLine(x,y0,x,y0+majorTickHeight_-1);
			
			// Tick labels
			if (usesTimeOfDay_)
			{
				int secs= ((int) rint(tickVal)) % 86400;
				int hh = secs / 3600;
				int mm = (secs - hh*3600) / 60;
				int ss = secs - hh*3600 - mm*60;
				tickLabel.sprintf("%02d:%02d:%02d",hh,mm,ss);
			}
			else
			{
				tickLabel =formatTick(tickVal,scalingExponent,tickSpacing);
			}
			labelWidth = fontMetrics().width(tickLabel,-1);

			if (i==0 && minTickLabel_.right() >= x-labelWidth/2)
				continue;
			else if (i==nIntervals && x + labelWidth/2 >= maxTickLabel_.left())
				continue;
		
			p.drawText(x-labelWidth/2,y0+ tickLabelOffset_+majorTickHeight_,labelWidth,labelHeight,Qt::AlignHCenter | Qt::AlignTop,tickLabel);
			
		}

		// title
		//struct timeval tv;
		//gettimeofday(&tv,NULL);
		// Nothing slow here - runs in 40 us
		int tw = title_->size().width();
		int th = title_->size().height();
		p.translate((w-tw)/2,h-th-1);
		if (scalingExponent != 0)
		{
			QString tmp;
			tmp.sprintf(" [10<sup>%i</sup>]",scalingExponent);
			tmp.prepend(titleText_);
			title_->setHtml(tmp);
		}
		title_->drawContents(&p);
		//struct timeval tv2;
		//gettimeofday(&tv2,NULL);
		//std::cerr << (tv2.tv_sec - tv.tv_sec)*1.0E6 + (tv2.tv_usec - tv.tv_usec) << std::endl;
		 //std::cerr << title_->size().width() << " " << title_->size().height() << std::endl;
	}
	
	if (geometryDirty_) // there has been a resize event
	{
		if (NULL != minInput_)  setMinTickInputPosition();
		if (NULL != maxInput_)  setMaxTickInputPosition();
		geometryDirty_=false;
	}
}

void AxisWidget::mousePressEvent ( QMouseEvent *me  )
{
	switch (me->button())
	{
		case Qt::LeftButton:
			break;
		case Qt::MidButton:
			break;
		case Qt::RightButton:
			createContextMenu(me->pos());
			break;
		default:
			break;
	}
}

void AxisWidget::mouseMoveEvent ( QMouseEvent * )
{
}

void AxisWidget::mouseReleaseEvent ( QMouseEvent * )
{
}

void AxisWidget::mouseDoubleClickEvent ( QMouseEvent *me )
{

	if ((NULL==minInput_) && (minTickLabel_.contains(me->pos())))
	{
	
		minInput_ = new DoubleInput(this);
		minInput_->setRange(axis_->minLimit(),axis_->maxLimit());
		minInput_->setValue(axis_->min());
		connect(minInput_,SIGNAL(destroyed(QObject*)),this,SLOT(firstTickInputClosed(QObject *)));
		connect(minInput_,SIGNAL(valueSet(double)),this,SLOT(setMin(double)));
		
		//QPoint pos = minTickLabel_.topLeft();
		minInput_->show();
		setMinTickInputPosition();
		
		//ali->setFocus();
	}
	else if ((NULL==maxInput_) && maxTickLabel_.contains(me->pos()))
	{
	
		maxInput_ = new DoubleInput(this);
		maxInput_->setRange(axis_->minLimit(),axis_->maxLimit());
		maxInput_->setValue(axis_->max());
		connect(maxInput_,SIGNAL(destroyed(QObject*)),this,SLOT(lastTickInputClosed(QObject *)));
		connect(maxInput_,SIGNAL(valueSet(double)),this,SLOT(setMax(double)));
		//QPoint pos=maxTickLabel_.topLeft();
		maxInput_->show();
		setMaxTickInputPosition();
		
		//ali->setFocus();
	}
	
}


void AxisWidget::createContextMenu(const QPoint &)
{
	QMenu *menu= new QMenu();
	
	if (orientation_ == Qt::Vertical){
		 menu->addAction("Min");
		 menu->addAction("Max");
	}
	else{
		 menu->addAction("Range");
	}
	
	menu->addAction("Axis min");
	
	menu->exec(QCursor::pos());	
	delete menu;
	
}

//
//
//


void AxisWidget::firstTickInputClosed(QObject *)
{
	minInput_=NULL;
}

void AxisWidget::lastTickInputClosed(QObject *)
{
	maxInput_=NULL;
}
		
void AxisWidget::init()
{
	
	orientation_=Qt::Vertical;
	style_=Axis::Linear;
	majorTickHeight_=7;
	minorTickHeight_=3;
	tickLabelOffset_=5;
	usesTimeOfDay_=false;
	autoScale_=true;
	
	minInput_=NULL;
	maxInput_=NULL;
	geometryDirty_=false;
}


void AxisWidget::setMinTickInputPosition()
{
	QPoint pos = minTickLabel_.topLeft();
	if (orientation_ == Qt::Vertical)
	{
			int dx = minTickLabel_.width() - minInput_->width();
			pos.setX(minTickLabel_.x() + ((dx<0)?dx:0));
			pos.setY(minTickLabel_.y() - minInput_->height()/2);
	}
	minInput_->hide();
	minInput_->move(pos);
	minInput_->show();
}

void AxisWidget::setMaxTickInputPosition()
{
	QPoint pos;
	if (orientation_ == Qt::Horizontal)
	{
			pos.setY(maxTickLabel_.y());
			int dx = maxTickLabel_.width() - maxInput_->width();
			pos.setX(maxTickLabel_.x() + ((dx<0)?dx:0));
	}
	else
	{
		pos = maxTickLabel_.topLeft();
		int dx = maxTickLabel_.width() - maxInput_->width();
		pos.setX(maxTickLabel_.x() + ((dx<0)?dx:0));
	}
	maxInput_->hide();
	maxInput_->move(pos);
	maxInput_->show();
}


QString AxisWidget::formatTick(double x,int e,double ts)
{
	
	QString r,fs;
	double d=1.0;
	
	if (e >= -1 && e < 4)
		d = 1.0;
	else
		d = pow(10.0,e);
	int xp=1;
	if (x != 0)
		xp = ceil(fabs(log10(fabs(x))))+1; // digits before DP
	int tsp=0;
	tsp =  ceil(fabs(log10(fabs(ts))))+1; // digits after DP
	qDebug() << x << " " << e << " " << d << " " << xp << " " << tsp;
	fs = "%."+QString::number(xp+tsp)+"g";
	r.sprintf(fs.toLatin1(),x/d);
	
	return r;
}


