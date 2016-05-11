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

#include <QLayout>
#include <QLabel>

#include "Key.h"
#include "KeyLabel.h"
#include "Series.h"
#include "StripChart.h"

Key::Key(Qt::Orientation o,QWidget *parent):QWidget(parent)
{
	init();
	if (o==Qt::Horizontal)
		b_ = new QHBoxLayout(this);
	else
		b_ = new QVBoxLayout(this);
	b_->setContentsMargins(2,2,2,2);
	b_->addStretch(1);
	
}

Key::~Key()
{
	 while (!labels_.isEmpty())
     delete labels_.takeFirst();
}

void Key::addSeries(Series *d)
{
	KeyLabel *kl = new KeyLabel(d,this);
	connect(kl,SIGNAL(attributesChanged(Series *)),
		this,SIGNAL(attributesChanged(Series *)));
	labels_.push_back(kl);
	b_->addWidget(kl,0);
}

void Key::init()
{
}
