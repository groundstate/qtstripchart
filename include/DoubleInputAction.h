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

#ifndef __DOUBLE_INPUT_ACTION_H_
#define __DOUBLE_INPUT_ACTION_H_

#include <QDoubleSpinBox>
#include <QLabel>
#include <QHBoxLayout>
#include <QWidget>
#include <QWidgetAction>

class DoubleInputAction:public QWidgetAction
{
	Q_OBJECT 
	
	public:
		
		DoubleInputAction(QString label,QObject *parent):QWidgetAction(parent)
		{label_=label;}
	
		double value(){return sb_->value();}
		
	signals:
		
		void valueChanged(double);
		
	protected:
		
		QWidget *createWidget(QWidget *parent)
		{
			QWidget *w = new QWidget(parent);
			QHBoxLayout *hb = new QHBoxLayout(w);
			QLabel  *l = new QLabel(label_,w);
			hb->addWidget(l,1);
			sb_ = new QDoubleSpinBox(w);
			hb->addWidget(sb_,1);
			connect(sb_,SIGNAL(valueChanged(double)),this,SIGNAL(valueChanged(double)));
			return w;
		}
		
	private:
		
		QString label_;
		QDoubleSpinBox * sb_;
};

#endif