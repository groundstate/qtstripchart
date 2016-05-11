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

#ifndef __AXIS_WIDGET_H_
#define __AXIS_WIDGET_H_

#include <QWidget>

#include "Axis.h"

class QTextDocument;

class Axis;
class DoubleInput;

class AxisWidget:public QWidget
{
	Q_OBJECT
	
	public:
	
		AxisWidget(int offset, Axis *,QWidget *parent=0);
		~AxisWidget();

		void setOrientation(Qt::Orientation);
		Qt::Orientation orientation(){return orientation_;}
		
		void setTitle(QString);
		
		void setMajorTickHeight(int h){ majorTickHeight_=h;}
		void setMinorTickHeight(int h){ minorTickHeight_=h;}
	
		QString formatValue(double);

	public slots:

		
		void setRange(double,double);
		
	signals:
		
		void rangeChanged(double,double);
		
	protected:

		virtual void resizeEvent(QResizeEvent *);
		virtual void paintEvent(QPaintEvent *);

		virtual void mousePressEvent ( QMouseEvent *  );
		virtual void mouseMoveEvent ( QMouseEvent * );
		virtual void mouseReleaseEvent ( QMouseEvent * );
		virtual void mouseDoubleClickEvent ( QMouseEvent *  );
		
	private slots:
	
		void setMin(double);
		void setMax(double);
		
		void createContextMenu(const QPoint &);
		void firstTickInputClosed(QObject *);
		void lastTickInputClosed(QObject *);
		
	private:

		void init();
		void setTicks();
		void setMinTickInputPosition();
		void setMaxTickInputPosition();
		QString formatTick(double ,int ,double );
		
		Axis * axis_;
		
		QTextDocument *title_; // in QT >= 4.7 can use a QStaticText
		QString titleText_;
		
		Qt::Orientation orientation_;
		//double dataMin_,dataMax_;
		Axis::AxisStyle style_;
		
		bool autoScale_;
		
		int majorTickHeight_;
		int minorTickHeight_;
		int tickLabelOffset_;
		int offset_; // offset of axis from plotting region
		bool usesTimeOfDay_;

		int w_,h_;
		
		QRect minTickLabel_,maxTickLabel_;
		DoubleInput *minInput_,*maxInput_;
		bool geometryDirty_;
};


#endif

