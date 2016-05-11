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

#include <QPainter>

#include "KeyColour.h"

KeyColour::KeyColour(QColor col,QWidget *parent):QWidget(parent)
{
	setMinimumSize(24,12);
	colour_=col;
	setContentsMargins(0,0,0,0);
}


void KeyColour::setColour(QColor col)
{
	colour_=col;
	repaint();
}


//
// Protected:
//

void KeyColour::paintEvent(QPaintEvent *)
{
	QPainter p(this);
	int w = width();
	int h = height();
	p.fillRect(0,0,w,h,QBrush(QColor("black")));

	QPen pen(colour_);
	pen.setWidth(3);
	p.setPen(pen);
	p.drawLine(2,h/2-1,w-3,h/2 -1);
}



