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

#include <iostream>
#include <cfloat>

#include <QWidget>
#include <QString>
#include <QKeyEvent>
#include <QEvent>
#include <QDoubleValidator>

#include "DoubleInput.h"

// Public members

DoubleInput::DoubleInput(QWidget *parent):
	QLineEdit(parent)
{
	v=0.0;
	QString t;
	setText(t.setNum(v));
	setMaxLength(12);
	int w = fontMetrics().width("99999999",-1);
	setMaximumWidth(w);
	validator= new QDoubleValidator(-DBL_MAX,DBL_MAX,8,this); // FIXME
	connect(this,SIGNAL(returnPressed()),this,SLOT(handleValueChange()));
}


DoubleInput::~DoubleInput()
{
	// Nothing to destroy
}


double DoubleInput::value()
{return text().toDouble();}	

		
void DoubleInput::setValue(double val)
{
	QString t;
	// Checking whether v has changed prevents an infinite loop
	// that can occur if this widget is bound up with another widget
	// like a slider
	if (v != val){
		if (val >= validator->bottom() && val <= validator->top()){
			setText(t.setNum(val));
			v=val;
			emit valueSet(v);
		}
	}
}


void DoubleInput::setRange(double min,double max)
{
	validator->setRange(min,max);
	if (v< min) v= min;
	if (v>max)  v= max;
}

void DoubleInput::keyPressEvent ( QKeyEvent * ev )
{
	if (Qt::Key_Escape == ev->key())
	{
		ev->accept();
		delete this;
	}
	else if ( Qt::Key_Return == ev->key() || Qt::Key_Enter == ev->key())
	{
		emit valueSet(value());
		ev->accept();
		delete this;
	}
	else
		QLineEdit::keyPressEvent(ev);
}

// Protected slots

//void DoubleInput::leaveEvent(QEvent *e)
//{
	// Dunno if this is standard behaviour but I think if you type
	// in something and then input focus is lost, the current input
	// ought to be validated. 
	 
//	QLineEdit::leaveEvent(e);
//	handleValueChange();
//}

// Private slots

void DoubleInput::handleValueChange()
{
	bool ok;
	double vtmp = text().toDouble(&ok);
	if (ok && vtmp != v){
		v=vtmp;
		emit valueSet(v);
	}
}
